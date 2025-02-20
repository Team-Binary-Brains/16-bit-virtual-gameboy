#include "univ_defs.h"
#include "O_token_types.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"
#include "O_inter_codegen.h"

void __printTokens(Token* tokens, size_t tokensIndex) {
    printf("+--------------------------+--------------------------+\n");
    printf("| %-24s | %-24s |\n", "Token Value", "Token Type");
    printf("+--------------------------+--------------------------+\n");
    for(size_t i=0; i<tokensIndex; i++){
        printf("| %-24s | %-24s |\n", tokens[i].value, StrTokenType[tokens[i].type]);
    }
    printf("+--------------------------+--------------------------+\n");
}

// For printing single token
void __printToken(Token token) {
    printf("+--------------------------+--------------------------+\n");
    printf("| %-24s | %-24s |\n", "Token Value", "Token Type");
    printf("+--------------------------+--------------------------+\n");
    printf("| %-24s | %-24s |\n", token.value, StrTokenType[token.type]);
    printf("+--------------------------+--------------------------+\n");
}

// Print the current scope for debugging
void printCurrentScope(SymbolTable* currentScope) {
    if (!currentScope) {
        printf("\nNo active scope available\n");
        return;
    }

    printf("-----------------------------------------------------\n");
    printf("| %-15s | %-10s | %-18s |\n", "Identifier", "Type", "Address");
    printf("-----------------------------------------------------\n");

    for (size_t i = 0; i < currentScope->table->size; i++) {
        Entry* entry = currentScope->table->entries[i];
        while (entry) {
            SymbolEntry* symEntry = (SymbolEntry*)entry->value;
            printf("| %-15s | %-10s | %-18p |\n", 
                   symEntry->identifier, StrTokenType[symEntry->type], symEntry->memAddress);
            entry = entry->next;
        }
    }

    printf("-----------------------------------------------------\n");
}

// Function to print the parse tree (preorder traversal)
void printParseTree(ParseTreeNode* node, char* prefix, int is_last) {
    if (!node) return;

    // Handle root node (no prefix yet)
    if (prefix == NULL) {
        printf("\nParse Tree:\n");
        printf("--------------------------------------\n");
        printf("%s\n", node->value);
        printCurrentScope(node->scope);

        char* new_prefix = (char*)malloc(256 * sizeof(char));
        strcpy(new_prefix, "");
        
        for (int i = 0; i < node->childCount; i++) {
            int child_is_last = (i == node->childCount - 1);
            printParseTree(node->children[i], new_prefix, child_is_last);
        }
        
        free(new_prefix);
        printf("\n");
        return;
    }

    // Print current node
    printf("%s", prefix);
    printf(is_last ? "L-- " : "|-- ");
    printf("%s\n", node->value);

    // Print the associated symbol table (if any)
    if (node->scope != NULL) {
        printf("\nSymbol Table for Scope in Node [%s]\n", node->value);
        printCurrentScope(node->scope);
    }

    // Prepare new prefix for children
    char* new_prefix = (char*)malloc(256 * sizeof(char));
    strcpy(new_prefix, prefix);
    strcat(new_prefix, is_last ? "    " : "|   ");

    // Recursively print children
    for (int i = 0; i < node->childCount; i++) {
        int child_is_last = (i == node->childCount - 1);
        printParseTree(node->children[i], new_prefix, child_is_last);
    }

    free(new_prefix);
}

// Print TAC instructions.
void printIntermediateCode(TACInstruction* tacHead) {
    TACInstruction* inst = tacHead;
    while (inst != NULL) {
        switch (inst->op) {
            case TAC_ASSIGN:
                printf("%s = %s\n", inst->result, inst->arg1);
                break;
            case TAC_ADD:
                printf("%s = %s + %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_SUB:
                printf("%s = %s - %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_MUL:
                printf("%s = %s * %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_DIV:
                printf("%s = %s / %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_MOD:
                printf("%s = %s %% %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_EQ:
                printf("%s = %s == %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_NE:
                printf("%s = %s != %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_LT:
                printf("%s = %s < %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_LE:
                printf("%s = %s <= %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_GT:
                printf("%s = %s > %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_GE:
                printf("%s = %s >= %s\n", inst->result, inst->arg1, inst->arg2);
                break;
            case TAC_LABEL:
                printf("%s:\n", inst->result);
                break;
            case TAC_GOTO:
                printf("goto %s\n", inst->arg1);
                break;
            case TAC_IF:
                printf("if %s goto %s\n", inst->arg1, inst->arg2);
                break;
            case TAC_PRINT:
                printf("print %s\n", inst->arg1);
                break;
            case TAC_SCAN:
                printf("scan %s\n", inst->arg1);
                break;
            case TAC_EXIT:
                printf("exit %s\n", inst->arg1);
                break;
            default:
                printf("Unknown TAC operation.\n");
        }
        inst = inst->next;
    }
}