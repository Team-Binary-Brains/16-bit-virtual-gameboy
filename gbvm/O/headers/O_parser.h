#pragma once
#include "O_lexer.h"

typedef struct ParseTreeNode {
    char* value; // Value of that node
    TokenType type; // Type of node
    struct ParseTreeNode** children; // Array of child nodes
    int childCount; // Number of children
} ParseTreeNode;

ParseTreeNode* parser(Token*);

ParseTreeNode* createParseTreeNode(Token*);

void addChild(ParseTreeNode*, ParseTreeNode*);

Token* match(Token** , TokenType);

ParseTreeNode* parseDeclaration(Token**);

ParseTreeNode* parseExpression(Token**);

ParseTreeNode* parseTerm(Token**);

ParseTreeNode* parseFactor(Token**);

ParseTreeNode* parseExitStatement(Token**);

void printParseTree(ParseTreeNode*, char*, int);

void freeParseTree(ParseTreeNode*);