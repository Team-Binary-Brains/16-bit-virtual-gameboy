#pragma once
#include "../headers/O#_lexer.h"

typedef struct Node{
    char *value;
    TokenType type;
    struct Node *right;
    struct Node *left;
}Node;



Node *parser(Token *tokens);
void __printTree(Node *node, int indent, char *identifier);
Node *initNode(Node *node, char *value, TokenType type);
Token* generateOperationNodes(Token *currentToken, Node* currentNode);

void printError(char *errorType, size_t line_number);
void handleTokenErrors(char *error_text, Token *currentToken, TokenType type);

Node *handleExitSyscall(Node *root, Token *currentToken, Node *current);
Node *createVariables(Token *currentToken, Node *current);