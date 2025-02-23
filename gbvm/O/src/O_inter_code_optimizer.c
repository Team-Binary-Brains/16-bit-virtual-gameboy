#include "univ_defs.h"
#include "O_inter_codegen.h"
#include "O_inter_code_optimizer.h"
#include "O_TAC_Oper.h"
#include "O_debug_help.h"

// Global TAC instruction list pointers.
static TACInstruction* tacHead;
static TACInstruction* tacTail;

//---------------------------------------------------------------------
// Helper functions

// Check if a string represents a constant integer.
static int isConstant(const char* s) {
    if (s == NULL || *s == '\0')
        return 0;
    int i = (s[0] == '-') ? 1 : 0;
    for (; s[i] != '\0'; i++) {
        if (!isdigit(s[i]))
            return 0;
    }
    return 1;
}

// Compute the result of a binary operation on two integer constants.
static int computeOperation(TacOp op, int left, int right) {
    switch (op) {
        case TAC_ADD: return left + right;
        case TAC_SUB: return left - right;
        case TAC_MUL: return left * right;
        case TAC_DIV: return right != 0 ? left / right : 0;
        case TAC_MOD: return right != 0 ? left % right : 0;
        case TAC_EQ:  return (left == right) ? 1 : 0;
        case TAC_NE:  return (left != right) ? 1 : 0;
        case TAC_LT:  return (left < right) ? 1 : 0;
        case TAC_LE:  return (left <= right) ? 1 : 0;
        case TAC_GT:  return (left > right) ? 1 : 0;
        case TAC_GE:  return (left >= right) ? 1 : 0;
        default:      return 0;
    }
}

// Replace occurrences of variable 'var' with 'replacement' in a TAC instruction.
static void replaceInInstruction(TACInstruction* inst, const char* var, const char* replacement) {
    if (inst->arg1 && strcmp(inst->arg1, var) == 0) {
        free(inst->arg1);
        inst->arg1 = strdup(replacement);
    }
    if (inst->arg2 && strcmp(inst->arg2, var) == 0) {
        free(inst->arg2);
        inst->arg2 = strdup(replacement);
    }
}

//---------------------------------------------------------------------
// Local Optimizations

// 1. Constant Folding: For operations with constant operands, compute the result.
void constantFoldingOptimization(void) {
    TACInstruction* inst = tacHead;
    while (inst != NULL) {
        if ((inst->op == TAC_ADD || inst->op == TAC_SUB ||
             inst->op == TAC_MUL || inst->op == TAC_DIV ||
             inst->op == TAC_MOD || inst->op == TAC_EQ  ||
             inst->op == TAC_NE  || inst->op == TAC_LT  ||
             inst->op == TAC_LE  || inst->op == TAC_GT  ||
             inst->op == TAC_GE) &&
            inst->arg1 && inst->arg2 &&
            isConstant(inst->arg1) && isConstant(inst->arg2)) {

            int left = atoi(inst->arg1);
            int right = atoi(inst->arg2);
            int result = computeOperation(inst->op, left, right);
            char buffer[32];
            sprintf(buffer, "%d", result);

            free(inst->arg1);
            free(inst->arg2);
            inst->arg1 = strdup(buffer);
            inst->arg2 = NULL;
            inst->op = TAC_ASSIGN;
        }
        inst = inst->next;
    }
}

// 2. Constant Propagation: Propagate constant assignments to later instructions.
void constantPropagation(void) {
    // For each assignment where RHS is a constant, replace subsequent uses.
    for (TACInstruction* inst = tacHead; inst != NULL; inst = inst->next) {
        if (inst->op == TAC_ASSIGN && isConstant(inst->arg1)) {
            char* var = inst->result;
            char* constVal = inst->arg1;
            // Traverse subsequent instructions.
            for (TACInstruction* nextInst = inst->next; nextInst != NULL; nextInst = nextInst->next) {
                // If the same variable is re-assigned, stop propagation.
                if (nextInst->op == TAC_ASSIGN && nextInst->result && strcmp(nextInst->result, var) == 0)
                    break;
                replaceInInstruction(nextInst, var, constVal);
            }
        }
    }
}

// 3. Copy Propagation: Propagate assignments that are simple copies.
void copyPropagation(void) {
    for (TACInstruction* inst = tacHead; inst != NULL; inst = inst->next) {
        // If assignment of the form X = Y, where Y is not a constant.
        if (inst->op == TAC_ASSIGN && inst->result && inst->arg1 && !isConstant(inst->arg1)) {
            char* target = inst->result;
            char* source = inst->arg1;
            // Propagate this copy to subsequent instructions.
            for (TACInstruction* nextInst = inst->next; nextInst != NULL; nextInst = nextInst->next) {
                // Stop if the target is redefined.
                if (nextInst->op == TAC_ASSIGN && nextInst->result && strcmp(nextInst->result, target) == 0)
                    break;
                replaceInInstruction(nextInst, target, source);
            }
        }
    }
}

// 4. Algebraic Simplification: Simplify expressions using algebraic identities.
void algebraicSimplification(void) {
    for (TACInstruction* inst = tacHead; inst != NULL; inst = inst->next) {
        // x + 0 or 0 + x  ==> x
        if (inst->op == TAC_ADD) {
            if (inst->arg1 && strcmp(inst->arg1, "0") == 0) {
                free(inst->arg1);
                inst->op = TAC_ASSIGN;
                // The value will come from arg2.
            } else if (inst->arg2 && strcmp(inst->arg2, "0") == 0) {
                free(inst->arg2);
                inst->op = TAC_ASSIGN;
                // The value will come from arg1.
            }
        }
        // x * 1 or 1 * x  ==> x
        if (inst->op == TAC_MUL) {
            if (inst->arg1 && strcmp(inst->arg1, "1") == 0) {
                free(inst->arg1);
                inst->op = TAC_ASSIGN;
            } else if (inst->arg2 && strcmp(inst->arg2, "1") == 0) {
                free(inst->arg2);
                inst->op = TAC_ASSIGN;
            }
        }
        // x - 0 ==> x
        if (inst->op == TAC_SUB) {
            if (inst->arg2 && strcmp(inst->arg2, "0") == 0) {
                free(inst->arg2);
                inst->op = TAC_ASSIGN;
            }
        }
        // x / 1 ==> x
        if (inst->op == TAC_DIV) {
            if (inst->arg2 && strcmp(inst->arg2, "1") == 0) {
                free(inst->arg2);
                inst->op = TAC_ASSIGN;
            }
        }
    }
}

// 5. Dead Code Elimination: Remove assignments whose result is never used.
void deadCodeElimination(void) {
    TACInstruction *prev = NULL, *curr = tacHead;
    while (curr != NULL) {
        int used = 0;
        // Look ahead for usage of curr->result.
        if (curr->result) {
            for (TACInstruction* temp = curr->next; temp != NULL; temp = temp->next) {
                if ((temp->arg1 && strcmp(temp->arg1, curr->result) == 0) ||
                    (temp->arg2 && strcmp(temp->arg2, curr->result) == 0)) {
                    used = 1;
                    break;
                }
            }
        }
        if (!used && curr->op == TAC_ASSIGN && curr->result) {
            // Remove curr from list.
            if (prev == NULL) {
                tacHead = curr->next;
                free(curr->result);
                if (curr->arg1) free(curr->arg1);
                if (curr->arg2) free(curr->arg2);
                TACInstruction* toDelete = curr;
                curr = curr->next;
                free(toDelete);
                continue;
            } else {
                prev->next = curr->next;
                if (curr == tacTail)
                    tacTail = prev;
                free(curr->result);
                if (curr->arg1) free(curr->arg1);
                if (curr->arg2) free(curr->arg2);
                TACInstruction* toDelete = curr;
                curr = curr->next;
                free(toDelete);
                continue;
            }
        }
        prev = curr;
        curr = curr->next;
    }
}

// 6. Strength Reduction: Replace expensive operations with cheaper ones (e.g., multiplication by 2 with addition).
void strengthReduction(void) {
    for (TACInstruction* inst = tacHead; inst != NULL; inst = inst->next) {
        if (inst->op == TAC_MUL) {
            // If one operand is "2", replace x * 2 with x + x.
            if (inst->arg1 && strcmp(inst->arg1, "2") == 0) {
                free(inst->arg1);
                inst->op = TAC_ADD;
                // Both operands become the original arg2.
                inst->arg1 = strdup(inst->arg2);
            } else if (inst->arg2 && strcmp(inst->arg2, "2") == 0) {
                free(inst->arg2);
                inst->op = TAC_ADD;
                inst->arg2 = strdup(inst->arg1);
            }
        }
    }
}

// 7. Peephole Optimization: Examine short instruction sequences to remove redundancies.
void peepholeOptimization(void) {
    TACInstruction *prev = NULL, *curr = tacHead;
    while (curr != NULL && curr->next != NULL) {
        // Example: if two consecutive assignments assign to the same variable,
        // remove the earlier one.
        if (prev && prev->op == TAC_ASSIGN && curr->op == TAC_ASSIGN &&
            prev->result && curr->result &&
            strcmp(prev->result, curr->result) == 0) {
            TACInstruction* toDelete = prev;
            if (tacHead == prev) {
                tacHead = curr;
            }
            // Here we mark the previous instruction as a no-op by converting it to a dummy GOTO.
            toDelete->op = TAC_GOTO;
            toDelete->arg1 = strdup(curr->result); // dummy replacement
        }
        prev = curr;
        curr = curr->next;
    }
}

// 8. Common Subexpression Elimination (CSE): Replace duplicate expressions with the first computed temporary.
void commonSubexpressionElimination(void) {
    // A simple O(n^2) approach: for each arithmetic instruction, search earlier instructions.
    for (TACInstruction* inst = tacHead; inst != NULL; inst = inst->next) {
        if (inst->op == TAC_ADD || inst->op == TAC_SUB ||
            inst->op == TAC_MUL || inst->op == TAC_DIV ||
            inst->op == TAC_MOD || inst->op == TAC_EQ  ||
            inst->op == TAC_NE  || inst->op == TAC_LT  ||
            inst->op == TAC_LE  || inst->op == TAC_GT  ||
            inst->op == TAC_GE) {

            // Search backwards.
            for (TACInstruction* prev = tacHead; prev != inst; prev = prev->next) {
                if (prev->op == inst->op &&
                    prev->arg1 && inst->arg1 && strcmp(prev->arg1, inst->arg1) == 0 &&
                    prev->arg2 && inst->arg2 && strcmp(prev->arg2, inst->arg2) == 0) {
                    // Replace current instruction with a copy of the earlier result.
                    free(inst->arg1);
                    if (inst->arg2) { free(inst->arg2); inst->arg2 = NULL; }
                    inst->op = TAC_ASSIGN;
                    inst->arg1 = strdup(prev->result);
                    break;
                }
            }
        }
    }
}

//---------------------------------------------------------------------
// Stub functions for Global / Interprocedural and Machine-Level Optimizations

void loopInvariantCodeMotion(void) {
    printf("Loop Invariant Code Motion: Not implemented (stub).\n");
}

void loopUnrolling(void) {
    printf("Loop Unrolling: Not implemented (stub).\n");
}

void loopFusion(void) {
    printf("Loop Fusion: Not implemented (stub).\n");
}

void registerAllocation(void) {
    printf("Register Allocation: Not implemented (stub).\n");
}

void functionInlining(void) {
    printf("Function Inlining: Not implemented (stub).\n");
}

void interproceduralConstantPropagation(void) {
    printf("Interprocedural Constant Propagation: Not implemented (stub).\n");
}

void instructionScheduling(void) {
    printf("Instruction Scheduling: Not implemented (stub).\n");
}

void codeLayoutOptimization(void) {
    printf("Code Layout Optimization: Not implemented (stub).\n");
}

//---------------------------------------------------------------------
// Main driver: optimizeCode calls all passes.
TACInstruction** optimizeCode(TACInstruction** tacList) {
    printf("\n--------------------------------------");
    printf("\nOptimized Code\n");
    printf("--------------------------------------\n");
    tacHead = tacList[0];
    tacTail = tacList[1];
    // Run local optimizations.
    constantFoldingOptimization();
    constantPropagation();
    copyPropagation();
    algebraicSimplification();
    deadCodeElimination();
    strengthReduction();
    peepholeOptimization();
    commonSubexpressionElimination();

    // Run global / advanced optimizations (stubs).
    loopInvariantCodeMotion();
    loopUnrolling();
    loopFusion();
    registerAllocation();
    functionInlining();
    interproceduralConstantPropagation();
    instructionScheduling();
    codeLayoutOptimization();

    printIntermediateCode(tacHead);
    tacList[0] = tacHead;
    tacList[1] = tacTail;
    return tacList;
}