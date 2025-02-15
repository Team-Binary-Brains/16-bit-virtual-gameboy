#pragma once
#include "O_token_types.h"
#include "O_symbol_table.h"

typedef struct ParseTreeNode {
    char* value; // Value of that node
    TokenType type; // Type of node
    struct ParseTreeNode** children; // Array of child nodes
    int childCount; // Number of children
    SymbolTable* scope;
} ParseTreeNode;

ParseTreeNode* createParseTreeNode(Token*, SymbolTable*);

void addChild(ParseTreeNode*, ParseTreeNode*);

void printParseTree(ParseTreeNode*, char*, int);

void freeParseTree(ParseTreeNode*);