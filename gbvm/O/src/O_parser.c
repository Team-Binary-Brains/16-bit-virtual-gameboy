#include "univ_defs.h"
#include "O_lexer.h"
#include "O_parser.h"

void printError(char* errorType, size_t lineNumber)
{
    printf("ERROR: %s on line number: %zu\n", errorType, lineNumber);
    exit(1);
}

void handleTokenErrors(char* errorText, Token* currentToken, TokenType type)
{
    if (currentToken->type == END_OF_TOKENS || currentToken->type != type) {
        printError(errorText, currentToken->lineNum);
    }
}

void __printTree(Node* node, int indent, char* identifier)
{
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("%s -> ", identifier);
    for (size_t i = 0; node->value[i] != '\0'; i++) {
        printf("%c", node->value[i]);
    }
    printf("\n");
    __printTree(node->left, indent + 1, "left");
    __printTree(node->right, indent + 1, "right");
}

Node* initNode(Node* node, char* value, TokenType type)
{
    node = malloc(sizeof(Node));
    node->value = malloc(sizeof(char) * 2);
    node->type = (int)type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Token* generateOperationNodes(Token* currentToken, Node* currentNode)
{
    Node* operNode = malloc(sizeof(Node));
    operNode = initNode(operNode, currentToken->value, OPERATOR);
    currentNode->left->left = operNode;
    currentNode = operNode;

    currentToken--;
    if (currentToken->type == INT) {
        Node* exprNode = malloc(sizeof(Node));
        exprNode = initNode(exprNode, currentToken->value, INT);
        currentNode->left = exprNode;
    } else if (currentToken->type == IDENTIFIER) {
        Node* identifier_node = malloc(sizeof(Node));
        identifier_node = initNode(identifier_node, currentToken->value, IDENTIFIER);
        currentNode->left = identifier_node;
    } else {
        printf("ERROR: expected int or identifier\n");
        exit(1);
    }

    currentToken++;
    currentToken++;
    while (currentToken->type == INT || currentToken->type == IDENTIFIER || currentToken->type == OPERATOR) {
        if (currentToken->type == INT || currentToken->type == IDENTIFIER) {
            if ((currentToken->type != INT && currentToken->type != IDENTIFIER) || currentToken == NULL) {
                printf("Syntax Error here\n");
                exit(1);
            }
            currentToken++;
            if (currentToken->type != OPERATOR) {
                currentToken--;
                if (currentToken->type == INT) {
                    Node* secondExprNode = malloc(sizeof(Node));
                    secondExprNode = initNode(secondExprNode, currentToken->value, INT);
                    currentNode->right = secondExprNode;
                } else if (currentToken->type == IDENTIFIER) {
                    Node* secondIdentifierNode = malloc(sizeof(Node));
                    secondIdentifierNode = initNode(secondIdentifierNode, currentToken->value, IDENTIFIER);
                    currentNode->right = secondIdentifierNode;
                } else {
                    printf("ERROR: Expected Integer or Identifier\n");
                    exit(1);
                }
            }
        }
        if (currentToken->type == OPERATOR) {
            Node* nextOperNode = malloc(sizeof(Node));
            nextOperNode = initNode(nextOperNode, currentToken->value, OPERATOR);
            currentNode->right = nextOperNode;
            currentNode = nextOperNode;
            currentToken--;
            if (currentToken->type == INT) {
                Node* secondExprNode = malloc(sizeof(Node));
                secondExprNode = initNode(secondExprNode, currentToken->value, INT);
                currentNode->left = secondExprNode;
            } else if (currentToken->type == IDENTIFIER) {
                Node* secondIdentifierNode = malloc(sizeof(Node));
                secondIdentifierNode = initNode(secondIdentifierNode, currentToken->value, IDENTIFIER);
                currentNode->left = secondIdentifierNode;
            } else {
                printf("ERROR: Expected IDENTIFIER or INT\n");
                exit(1);
            }
            currentToken++;
        }
        currentToken++;
    }
    return currentToken;
}

Node* handleExitSyscall(Node* root, Token* currentToken, Node* current)
{
    Node* exitNode = malloc(sizeof(Node));
    exitNode = initNode(exitNode, currentToken->value, KEYWORD);
    current->right = exitNode;
    current = exitNode;
    currentToken++;

    handleTokenErrors("Invalid Syntax on OPEN", currentToken, SEPARATOR);
    if (strcmp(currentToken->value, "(") == 0) {     // Improvement
        Node* openParenNode = malloc(sizeof(Node));
        openParenNode = initNode(openParenNode, currentToken->value, SEPARATOR);
        current->left = openParenNode;
        currentToken++;
        if (currentToken->type == END_OF_TOKENS) {
            printError("Invalid Syntax on INT", currentToken->lineNum);
        }
        if (currentToken->type == INT || currentToken->type == IDENTIFIER) {
            currentToken++;
            if (currentToken->type == OPERATOR && currentToken != NULL) {
                currentToken = generateOperationNodes(currentToken, current);
                currentToken--;
            } else {
                currentToken--;
                Node* exprNode = malloc(sizeof(Node));
                exprNode = initNode(exprNode, currentToken->value, currentToken->type);
                current->left->left = exprNode;
            }
            currentToken++;
            handleTokenErrors("Invalid Syntax on CLOSE", currentToken, SEPARATOR);
            if (strcmp(currentToken->value, ")") == 0) {     // Improvement
                Node* closeParenNode = malloc(sizeof(Node));
                closeParenNode = initNode(closeParenNode, currentToken->value, SEPARATOR);
                current->left->right = closeParenNode;
                currentToken++;

                handleTokenErrors("Invalid Syntax on SEMI", currentToken, SEPARATOR);

                if (strcmp(currentToken->value, ";") == 0) {
                    Node* semiNode = malloc(sizeof(Node));
                    semiNode = initNode(semiNode, currentToken->value, SEPARATOR);
                    current->right = semiNode;
                    current = semiNode;
                } else {
                    printError("Invalid Syntax on SEMI", currentToken->lineNum);
                }
            } else {
                printError("Invalid Syntax on CLOSE", currentToken->lineNum);
            }
        } else {
            printError("Invalid Syntax on INT", currentToken->lineNum);
        }

    } else {
        printError("Invalid Syntax on OPEN", currentToken->lineNum);
    }
    return current;
}

Node* createVariables(Token* currentToken, Node* current)
{
    Node* varNode = malloc(sizeof(Node));
    varNode = initNode(varNode, currentToken->value, KEYWORD);
    current->left = varNode;
    current = varNode;
    currentToken++;

    handleTokenErrors("Invalid syntax after INT", currentToken, IDENTIFIER);     // improvement
    Node* identifierNode = malloc(sizeof(Node));
    identifierNode = initNode(identifierNode, currentToken->value, IDENTIFIER);
    current->left = identifierNode;
    current = identifierNode;
    currentToken++;

    handleTokenErrors("Invalid syntax after Identifier", currentToken, OPERATOR);
    if (strcmp(currentToken->value, "=") != 0) {
        printError("Error: Invalid Variable Syntax on =", currentToken->lineNum);
    }
    Node* equals_node = malloc(sizeof(Node));
    equals_node = initNode(equals_node, currentToken->value, OPERATOR);
    current->left = equals_node;
    current = equals_node;
    currentToken++;

    if (currentToken->type == END_OF_TOKENS || (currentToken->type != INT && currentToken->type != IDENTIFIER)) {
        printError("ERROR: Invalid Syntax After Equals", currentToken->lineNum);
    }

    currentToken++;
    if (currentToken->type == OPERATOR) {
        Node* operNode = malloc(sizeof(Node));
        operNode = initNode(operNode, currentToken->value, OPERATOR);
        current->left = operNode;
        current = operNode;
        currentToken--;
        if (currentToken->type == INT) {
            Node* exprNode = malloc(sizeof(Node));
            exprNode = initNode(exprNode, currentToken->value, INT);
            currentToken++;
            currentToken++;
        } else if (currentToken->type == IDENTIFIER) {
            Node* identifierNode = malloc(sizeof(Node));
            identifierNode = initNode(identifierNode, currentToken->value, IDENTIFIER);
            operNode->left = identifierNode;
            currentToken++;
            currentToken++;
        } else {
            printError("ERROR: Expected IDENTIFIER or INT", currentToken->lineNum);
        }

        currentToken++;

        if (currentToken->type == OPERATOR) {
            Node* operNode = malloc(sizeof(Node));
            operNode = initNode(operNode, currentToken->value, OPERATOR);
            current->right = operNode;
            current = operNode;
            int operation = 1;
            currentToken--;
            currentToken--;
            while (operation) {
                currentToken++;
                if (currentToken->type == INT) {
                    Node* exprNode = malloc(sizeof(Node));
                    exprNode = initNode(exprNode, currentToken->value, INT);
                    current->left = exprNode;
                } else if (currentToken->type == IDENTIFIER) {
                    Node* identifier_node = malloc(sizeof(Node));
                    identifier_node = initNode(identifier_node, currentToken->value, IDENTIFIER);
                    current->left = identifier_node;
                } else {
                    printf("ERROR: Unexpected Token\n");
                    exit(1);
                }
                currentToken++;
                if (currentToken->type == OPERATOR) {
                    currentToken++;
                    currentToken++;
                    if (currentToken->type != OPERATOR) {
                        currentToken--;
                        if (currentToken->type == INT) {
                            Node* exprNode = malloc(sizeof(Node));
                            exprNode = initNode(exprNode, currentToken->value, INT);
                            current->right = exprNode;
                            currentToken++;
                        } else if (currentToken->type == IDENTIFIER) {
                            Node* identifier_node = malloc(sizeof(Node));
                            identifier_node = initNode(identifier_node, currentToken->value, IDENTIFIER);
                            current->right = identifier_node;
                            currentToken++;
                        } else {
                            printf("ERROR: UNRECOGNIZED TOKEN!\n");
                            exit(1);
                        }
                        operation = 0;
                    } else {
                        currentToken--;
                        currentToken--;
                        Node* operNode = malloc(sizeof(Node));
                        operNode = initNode(operNode, currentToken->value, OPERATOR);
                        current->right = operNode;
                        current = operNode;
                    }
                } else {
                    operation = 0;
                }
            }

        } else {
            currentToken--;
            if (currentToken->type == INT) {
                Node* exprNode = malloc(sizeof(Node));
                exprNode = initNode(exprNode, currentToken->value, INT);
                operNode->right = exprNode;
            } else if (currentToken->type == IDENTIFIER) {
                Node* identifier_node = malloc(sizeof(Node));
                identifier_node = initNode(identifier_node, currentToken->value, IDENTIFIER);
                operNode->right = identifier_node;
            }
            currentToken++;
        }

    } else {
        currentToken--;
        if (currentToken->type == INT) {
            Node* exprNode = malloc(sizeof(Node));
            exprNode = initNode(exprNode, currentToken->value, INT);
            current->left = exprNode;
            currentToken++;
        } else if (currentToken->type == IDENTIFIER) {
            Node* identifier_node = malloc(sizeof(Node));
            identifier_node = initNode(identifier_node, currentToken->value, IDENTIFIER);
            current->left = identifier_node;
            currentToken++;
        }
    }

    handleTokenErrors("Invalid Syntax After Expression", currentToken, SEPARATOR);

    current = varNode;
    if (strcmp(currentToken->value, ";") == 0) {
        Node* semiNode = malloc(sizeof(Node));
        semiNode = initNode(semiNode, currentToken->value, SEPARATOR);
        current->right = semiNode;
        current = semiNode;
    }
    return current;
}

Node* parser(Token* tokens)
{
    Token* currentToken = &tokens[0];
    Node* root = malloc(sizeof(Node));
    root = initNode(root, "PROGRAM", BEGINNING);

    Node* current = root;

    while (currentToken->type != END_OF_TOKENS) {
        if (current == NULL) {
            break;
        }

        switch (currentToken->type) {
        case KEYWORD:
            if (strcmp(currentToken->value, "EXIT") == 0) {
                current = handleExitSyscall(root, currentToken, current);
            } else if (strcmp(currentToken->value, "INT") == 0) {
                current = createVariables(currentToken, current);
            }
            break;
        }
        currentToken++;
        // printf("I am here");
    }
    __printTree(root, 0, "root");
    return root;
}