#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wswitch"

#include "univ_defs.h"
#include "O_lexer.h"
#include "O_parser.h"

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

// Match function to check token type and move to the next token
Token* match(Token** currentToken, TokenType expectedType) {
    if ((*currentToken)->type == expectedType) {
        Token* matchedToken = *currentToken;
        (*currentToken)++;
        return matchedToken;
    } else {
        printf("Syntax error: Expected token type %d but found %d\n", expectedType, (*currentToken)->type);
        exit(1);
    }
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

// Parse a factor (basic unit of an expression)
ParseTreeNode* parseFactor(Token** currentToken) {
    if ((*currentToken)->type == LPAREN) {
        // Match '('
        match(currentToken, LPAREN);
        ParseTreeNode* exprNode = parseExpression(currentToken);
        // Match ')'
        match(currentToken, RPAREN);
        return exprNode;
    } else if ((*currentToken)->type == IDENTIFIER) {
        return createParseTreeNode(match(currentToken, IDENTIFIER));
    } else if ((*currentToken)->type == INT_L) {
        return createParseTreeNode(match(currentToken, INT_L));
    } else {
        printf("Syntax error: Unexpected token '%s' in factor at line %zu\n", (*currentToken)->value, (*currentToken)->lineNum);
        exit(1);
    }
}

// Parse term (handles *, /, and %)
ParseTreeNode* parseTerm(Token** currentToken) {
    ParseTreeNode* left = parseFactor(currentToken);

    while ((*currentToken)->type == STAR || (*currentToken)->type == SLASH || (*currentToken)->type == MOD) {
        ParseTreeNode* operatorNode = createParseTreeNode(match(currentToken, (*currentToken)->type));
        addChild(operatorNode, left);
        addChild(operatorNode, parseFactor(currentToken));
        left = operatorNode;  // Update left for chaining
    }

    return left;
}


// Parse expression (handles + and -)
ParseTreeNode* parseExpression(Token** currentToken) {
    ParseTreeNode* left = parseTerm(currentToken);

    while ((*currentToken)->type == PLUS || (*currentToken)->type == MINUS) {
        ParseTreeNode* operatorNode = createParseTreeNode(match(currentToken, (*currentToken)->type));
        addChild(operatorNode, left);
        addChild(operatorNode, parseTerm(currentToken));
        left = operatorNode;  // Update left for chaining
    }

    return left;
}


// Parse declaration (e.g., int a = 8; or int a = 3 + 5;)
ParseTreeNode* parseDeclaration(Token** currentToken) {
    ParseTreeNode* declarationNode = createParseTreeNode(&(Token){.value = "DECLARATION", .type = BEGINNING});

    // Match 'int'
    addChild(declarationNode, createParseTreeNode(match(currentToken, INT)));

    // Match identifier
    addChild(declarationNode, createParseTreeNode(match(currentToken, IDENTIFIER)));

    // Match '='
    addChild(declarationNode, createParseTreeNode(match(currentToken, EQUAL)));

    // Parse expression
    addChild(declarationNode, parseExpression(currentToken));

    // Match ';'
    addChild(declarationNode, createParseTreeNode(match(currentToken, SEMICOLON)));

    return declarationNode;
}

// Parse exit statement (e.g., exit(3 + 5);)
ParseTreeNode* parseExitStatement(Token** currentToken) {
    ParseTreeNode* exitNode = createParseTreeNode(&(Token){.value = "EXIT_STATEMENT", .type = BEGINNING});

    // Match 'exit'
    addChild(exitNode, createParseTreeNode(match(currentToken, EXIT)));

    // Match '('
    addChild(exitNode, createParseTreeNode(match(currentToken, LPAREN)));

    // Parse expression
    addChild(exitNode, parseExpression(currentToken));

    // Match ')'
    addChild(exitNode, createParseTreeNode(match(currentToken, RPAREN)));

    // Match ';'
    addChild(exitNode, createParseTreeNode(match(currentToken, SEMICOLON)));

    return exitNode;
}

// Parser function
ParseTreeNode* parser(Token* tokens) {
    Token* currentToken = tokens;

    // Create the root of the parse tree
    ParseTreeNode* root = createParseTreeNode(&(Token){.value = "PROGRAM", .type = BEGINNING});

    while (currentToken->type != END_OF_TOKENS) {
        if (currentToken->type == INT) {
            // Parse a declaration
            addChild(root, parseDeclaration(&currentToken));
        } else if (currentToken->type == EXIT) {
            // Parse an exit statement
            addChild(root, parseExitStatement(&currentToken));
        } else {
            printf("Syntax error: Unexpected token '%s' at line %zu\n", currentToken->value, currentToken->lineNum);
            freeParseTree(root);
            exit(1);
        }
    }

    printf("Parse Tree:\n");
    printParseTree(root, NULL, 0);

    return root;
}