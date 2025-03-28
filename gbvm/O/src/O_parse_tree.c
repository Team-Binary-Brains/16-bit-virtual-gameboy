#include "univ_defs.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"

// Function to create a new parse tree node
ParseTreeNode* createParseTreeNode(Token* token, SymbolTable* scope) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    node->value = strdup(token->value);
    node->type = token->type;
    node->children = NULL;
    node->childCount = 0;
    node->scope = scope;
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