#include "univ_defs.h"
#include "O_parse_tree.h"

// Function to create a new parse tree node
ParseTreeNode* createParseTreeNode(Token* token) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    node->value = strdup(token->value);
    node->type = token->type;
    node->children = NULL;
    node->childCount = 0;
    return node;
}

// Function to add a child node to a parent
void addChild(ParseTreeNode* parent, ParseTreeNode* child) {
    parent->childCount++;
    parent->children = (ParseTreeNode**)realloc(parent->children, parent->childCount * sizeof(ParseTreeNode*));
    parent->children[parent->childCount - 1] = child;
}

// Function to free the parse tree
void freeParseTree(ParseTreeNode* root) {
    for (int i = 0; i < root->childCount; i++) {
        freeParseTree(root->children[i]);
    }
    free(root->value);
    free(root->children);
    free(root);
}

// Function to print the parse tree (preorder traversal)
void printParseTree(ParseTreeNode* node, char* prefix, int is_last) {
    if (!node) return;

    // Handle root node
    if (prefix == NULL) {
        printf("Parse Tree:\n");
        printf("%s\n", node->value);
        char* new_prefix = (char*)malloc(256 * sizeof(char));
        strcpy(new_prefix, "");
        for (int i = 0; i < node->childCount; i++) {
            int child_is_last = (i == node->childCount - 1);
            printParseTree(node->children[i], new_prefix, child_is_last);
        }
        free(new_prefix);
        return;
    }

    // Print current node
    printf("%s", prefix);
    printf(is_last ? "L-- " : "|-- ");
    printf("%s\n", node->value);

    // Prepare new prefix
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