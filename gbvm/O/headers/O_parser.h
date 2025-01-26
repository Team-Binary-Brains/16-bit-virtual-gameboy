/**
 * @file O_parser.h
 * @brief MISSING DOCUMENTATION.
 *
 * @author Omkar Jagtap
 * @date January 2025
 */

#pragma once
#include "O_lexer.h"

typedef struct Node {
    char* value;
    TokenType type;
    struct Node* right;
    struct Node* left;
} Node;

Node* parser(Token*);
void __printTree(Node*, char, int);
Node* initNode(Node*, char*, TokenType);
Token* generateOperationNodes(Token*, Node*);

void printError(char*, size_t);
void handleTokenErrors(char*, Token*, TokenType);

Node* handleExitSyscall(Node*, Token*, Node*);
Node* createVariables(Token*, Node*);