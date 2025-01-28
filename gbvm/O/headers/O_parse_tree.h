#pragma once
#include "O_token_types.h"

typedef struct ParseTreeNode {
    char* value; // Value of that node
    TokenType type; // Type of node
    struct ParseTreeNode** children; // Array of child nodes
    int childCount; // Number of children
} ParseTreeNode;

ParseTreeNode* createParseTreeNode(Token*);

void addChild(ParseTreeNode*, ParseTreeNode*);

void printParseTree(ParseTreeNode*, char*, int);

void freeParseTree(ParseTreeNode*);