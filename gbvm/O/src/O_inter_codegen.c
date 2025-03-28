#include "univ_defs.h"
#include "O_inter_codegen.h"
#include "O_parse_tree.h"
#include "O_token_types.h"
#include "O_debug_help.h"

// Global TAC instruction list pointers.
static TACInstruction* tacHead = NULL;
static TACInstruction* tacTail = NULL;

static TACInstruction* res[2];

// Counters for unique temporary variables and labels.
static int tempCount = 0;
static int labelCount = 0;

//---------------------------------------------------------------------
// Utility functions for generating names and instructions.

static char* newTemp(void)
{
    char buffer[32];
    sprintf(buffer, "t%d", tempCount++);
    return strdup(buffer);
}

static char* newLabel(void)
{
    char buffer[32];
    sprintf(buffer, "L%d", labelCount++);
    return strdup(buffer);
}

static TACInstruction* newTAC(TacOp op, char* result, char* arg1, char* arg2)
{
    TACInstruction* inst = (TACInstruction*)malloc(sizeof(TACInstruction));
    inst->op = op;
    inst->result = result ? strdup(result) : NULL;
    inst->arg1 = arg1 ? strdup(arg1) : NULL;
    inst->arg2 = arg2 ? strdup(arg2) : NULL;
    inst->next = NULL;
    return inst;
}

static void emit(TACInstruction* inst)
{
    if (tacHead == NULL) {
        tacHead = tacTail = inst;
    } else {
        tacTail->next = inst;
        tacTail = inst;
    }
}

//---------------------------------------------------------------------
// generateCode: recursively traverse the parse tree and emit TAC.
static void generateCode(ParseTreeNode* node)
{
    if (node == NULL)
        return;

    // Handle high-level statement nodes based on their special marker.
    if (strcmp(node->value, "PROGRAM") == 0) {
        // Root node: process every child statement.
        for (int i = 0; i < node->childCount; i++) {
            generateCode(node->children[i]);
        }
    } else if (strcmp(node->value, "DECLARATION") == 0) {
        // Declaration node: children[0]=DataType, children[1]=Identifier.
        // If an initializer exists, it is at children[3] (children[2] is the "=" token).
        if (node->childCount >= 4) {
            char* id = node->children[1]->value;
            // Generate code for the initializer expression.
            char* exprTemp = generateExpression(node->children[3]);
            emit(newTAC(TAC_ASSIGN, id, exprTemp, NULL));
        }
        // No TAC generated for plain declarations without initializer.
    } else if (strcmp(node->value, "ASSIGNMENT") == 0) {
        // Assignment node: children[0]=Identifier, children[1]= "=" token, children[2]= Expression.
        char* id = node->children[0]->value;
        char* exprTemp = generateExpression(node->children[2]);
        emit(newTAC(TAC_ASSIGN, id, exprTemp, NULL));
    } else if (strcmp(node->value, "PRINT_STATEMENT") == 0) {
        // Print statement: children[1] is the expression to print.
        char* exprTemp = generateExpression(node->children[1]);
        emit(newTAC(TAC_PRINT, NULL, exprTemp, NULL));
    } else if (strcmp(node->value, "SCAN_STATEMENT") == 0) {
        // Scan statement: children[1] is the identifier.
        char* id = node->children[1]->value;
        emit(newTAC(TAC_SCAN, NULL, id, NULL));
    } else if (strcmp(node->value, "EXIT_STATEMENT") == 0) {
        // Exit statement: children[2] is the expression.
        char* exprTemp = generateExpression(node->children[2]);
        emit(newTAC(TAC_EXIT, NULL, exprTemp, NULL));
    } else if (strcmp(node->value, "IF_STATEMENT") == 0) {
        // IF_STATEMENT: children[1]= condition, children[2]= then-statement,
        // optionally children[4]= else-statement if an ELSE branch exists.
        char* condTemp = generateExpression(node->children[1]);
        char* labelTrue = newLabel();
        char* labelFalse = newLabel();
        // if condition goto labelTrue
        emit(newTAC(TAC_IF, NULL, condTemp, labelTrue));
        // Otherwise, jump to false branch.
        emit(newTAC(TAC_GOTO, NULL, labelFalse, NULL));
        // True branch.
        emit(newTAC(TAC_LABEL, labelTrue, NULL, NULL));
        generateCode(node->children[2]);
        if (node->childCount >= 5) {
            // Else branch exists: children[3] is the "else" token; children[4] is the else-statement.
            char* labelEnd = newLabel();
            emit(newTAC(TAC_GOTO, NULL, labelEnd, NULL));
            emit(newTAC(TAC_LABEL, labelFalse, NULL, NULL));
            generateCode(node->children[4]);
            emit(newTAC(TAC_LABEL, labelEnd, NULL, NULL));
        } else {
            emit(newTAC(TAC_LABEL, labelFalse, NULL, NULL));
        }
    } else if (strcmp(node->value, "WHILE_STATEMENT") == 0) {
        // WHILE_STATEMENT: children[1] is condition, children[2] is loop body.
        char* labelStart = newLabel();
        char* labelExit = newLabel();
        emit(newTAC(TAC_LABEL, labelStart, NULL, NULL));
        char* condTemp = generateExpression(node->children[1]);
        char* labelBody = newLabel();
        emit(newTAC(TAC_IF, NULL, condTemp, labelBody));
        emit(newTAC(TAC_GOTO, NULL, labelExit, NULL));
        emit(newTAC(TAC_LABEL, labelBody, NULL, NULL));
        generateCode(node->children[2]);
        emit(newTAC(TAC_GOTO, NULL, labelStart, NULL));
        emit(newTAC(TAC_LABEL, labelExit, NULL, NULL));
    } else if (strcmp(node->value, "FOR_STATEMENT") == 0) {
        // FOR_STATEMENT: The children vary:
        // Child[0] is the "for" token.
        // Then, if an initializer exists, it will be present.
        // The condition expression is always present.
        // Optionally, an update node may be present.
        // The loop body is the last child.
        int cnt = node->childCount;
        ParseTreeNode *init = NULL, *cond = NULL, *update = NULL, *body = NULL;
        // Always, the first child is the "for" token.
        int index = 1;
        // If the next child is a declaration or assignment, treat it as the initializer.
        if (index < cnt && (strcmp(node->children[index]->value, "DECLARATION") == 0 || strcmp(node->children[index]->value, "ASSIGNMENT") == 0)) {
            init = node->children[index];
            index++;
        }
        // The condition expression is next.
        if (index < cnt) {
            cond = node->children[index++];
        }
        // If there is one more child before the final one, treat it as the update.
        if (index < cnt - 1) {
            update = node->children[index++];
        }
        // The last child is the loop body.
        if (index < cnt) {
            body = node->children[index];
        }
        // Generate code.
        if (init)
            generateCode(init);
        char* labelStart = newLabel();
        char* labelExit = newLabel();
        emit(newTAC(TAC_LABEL, labelStart, NULL, NULL));
        char* condTemp = generateExpression(cond);
        char* labelBody = newLabel();
        emit(newTAC(TAC_IF, NULL, condTemp, labelBody));
        emit(newTAC(TAC_GOTO, NULL, labelExit, NULL));
        emit(newTAC(TAC_LABEL, labelBody, NULL, NULL));
        generateCode(body);
        if (update)
            generateCode(update);
        emit(newTAC(TAC_GOTO, NULL, labelStart, NULL));
        emit(newTAC(TAC_LABEL, labelExit, NULL, NULL));
    } else if (strcmp(node->value, "BLOCK") == 0) {
        // BLOCK: simply generate code for each statement within the block.
        for (int i = 0; i < node->childCount; i++) {
            generateCode(node->children[i]);
        }
    } else {
        // For any node that does not match a high-level statement,
        // assume it is an expression or operator node.
        // In a statement context, we simply generate the expression,
        // but do not emit further TAC (unless its children contain statements).
        if (node->childCount > 0) {
            // Recursively process children.
            for (int i = 0; i < node->childCount; i++) {
                generateCode(node->children[i]);
            }
        }
    }
}

//---------------------------------------------------------------------
// generateExpression: returns the name of the temporary holding the result.
static char* generateExpression(ParseTreeNode* node)
{
    if (node == NULL)
        return NULL;

    // If a leaf node (identifier, literal), return its value.
    if (node->childCount == 0)
        return strdup(node->value);

    // For expression nodes (operators), assume binary operator.
    // (d are not explicitly represented because they were consumed.)
    char* left = generateExpression(node->children[0]);
    char* right = generateExpression(node->children[1]);
    char* temp = newTemp();

    if (strcmp(node->value, "+") == 0) {
        emit(newTAC(TAC_ADD, temp, left, right));
    } else if (strcmp(node->value, "-") == 0) {
        emit(newTAC(TAC_SUB, temp, left, right));
    } else if (strcmp(node->value, "*") == 0) {
        emit(newTAC(TAC_MUL, temp, left, right));
    } else if (strcmp(node->value, "/") == 0) {
        emit(newTAC(TAC_DIV, temp, left, right));
    } else if (strcmp(node->value, "%") == 0) {
        emit(newTAC(TAC_MOD, temp, left, right));
    } else if (strcmp(node->value, "==") == 0) {
        emit(newTAC(TAC_EQ, temp, left, right));
    } else if (strcmp(node->value, "!=") == 0) {
        emit(newTAC(TAC_NE, temp, left, right));
    } else if (strcmp(node->value, "<") == 0) {
        emit(newTAC(TAC_LT, temp, left, right));
    } else if (strcmp(node->value, "<=") == 0) {
        emit(newTAC(TAC_LE, temp, left, right));
    } else if (strcmp(node->value, ">") == 0) {
        emit(newTAC(TAC_GT, temp, left, right));
    } else if (strcmp(node->value, ">=") == 0) {
        emit(newTAC(TAC_GE, temp, left, right));
    } else {
        // If unrecognized, default to assignment.
        emit(newTAC(TAC_ASSIGN, temp, left, NULL));
    }

    return temp;
}

//---------------------------------------------------------------------
TACInstruction** generateIntermediateCode(ParseTreeNode* root)
{
    // Reset state.
    tempCount = 0;
    labelCount = 0;
    tacHead = tacTail = NULL;
    generateCode(root);
    // printIntermediateCode(tacHead);   FUNCTION NOT IMPLEMENTED
    res[0] = tacHead;
    res[1] = tacTail;
    return res;
}

// Free the TAC instruction list.
void freeIntermediateCode(void)
{
    TACInstruction* curr = tacHead;
    while (curr != NULL) {
        TACInstruction* temp = curr;
        curr = curr->next;
        if (temp->result)
            free(temp->result);
        if (temp->arg1)
            free(temp->arg1);
        if (temp->arg2)
            free(temp->arg2);
        free(temp);
    }
    tacHead = tacTail = NULL;
}
