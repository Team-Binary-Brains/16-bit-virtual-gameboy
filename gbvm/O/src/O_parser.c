#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wswitch"

#include "univ_defs.h"
#include "O_token_types.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"

// Forward declarations for parsing functions with symbol table usage.
ParseTreeNode* parseExpression(Token** currentToken, SymbolTable* symTable);
ParseTreeNode* parseTerm(Token** currentToken, SymbolTable* symTable);
ParseTreeNode* parseFactor(Token** currentToken, SymbolTable* symTable);
ParseTreeNode* parseDeclaration(Token** currentToken, SymbolTable* symTable);
ParseTreeNode* parseAssignment(Token** currentToken, SymbolTable* symTable);
ParseTreeNode* parseExitStatement(Token** currentToken, SymbolTable* symTable);

// Match function: verifies that the current token matches the expected type.
Token* match(Token** currentToken, TokenType expectedType) {
    if ((*currentToken)->type == expectedType) {
        Token* matchedToken = *currentToken;
        (*currentToken)++;
        return matchedToken;
    } else {
        printf("Syntax error: Expected token type %d but found %d at line %zu\n",
               expectedType, (*currentToken)->type, (*currentToken)->lineNum);
        exit(1);
    }
}

// Parse a factor: handles ( Expression ), identifiers, and integer literals.
ParseTreeNode* parseFactor(Token** currentToken, SymbolTable* symTable) {
    if ((*currentToken)->type == LPAREN) {
        match(currentToken, LPAREN);
        ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
        match(currentToken, RPAREN);
        return exprNode;
    } else if ((*currentToken)->type == IDENTIFIER) {
        // Check if the variable has been declared.
        if (!lookupSymbol(symTable, (*currentToken)->value)) {
            printf("Semantic error: Undeclared variable '%s' at line %zu\n",
                   (*currentToken)->value, (*currentToken)->lineNum);
            exit(1);
        }
        return createParseTreeNode(match(currentToken, IDENTIFIER));
    } else if ((*currentToken)->type == INT_L) {
        return createParseTreeNode(match(currentToken, INT_L));
    } else {
        printf("Syntax error: Unexpected token '%s' in factor at line %zu\n",
               (*currentToken)->value, (*currentToken)->lineNum);
        exit(1);
    }
}

// Parse a term: handles multiplication, division, and modulo operations.
ParseTreeNode* parseTerm(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parseFactor(currentToken, symTable);

    while ((*currentToken)->type == STAR || (*currentToken)->type == SLASH ||
           (*currentToken)->type == MOD) {
        TokenType opType = (*currentToken)->type;
        ParseTreeNode* operatorNode = createParseTreeNode(match(currentToken, opType));
        addChild(operatorNode, left);
        addChild(operatorNode, parseFactor(currentToken, symTable));
        left = operatorNode;
    }
    return left;
}

// Parse an expression: handles addition and subtraction.
ParseTreeNode* parseExpression(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parseTerm(currentToken, symTable);

    while ((*currentToken)->type == PLUS || (*currentToken)->type == MINUS) {
        TokenType opType = (*currentToken)->type;
        ParseTreeNode* operatorNode = createParseTreeNode(match(currentToken, opType));
        addChild(operatorNode, left);
        addChild(operatorNode, parseTerm(currentToken, symTable));
        left = operatorNode;
    }
    return left;
}

// Parse a declaration: int IDENTIFIER = Expression ;
ParseTreeNode* parseDeclaration(Token** currentToken, SymbolTable* symTable) {
    // Create node for declaration statement.
    ParseTreeNode* declNode = createParseTreeNode(&(Token){.value="DECLARATION", .type=BEGINNING});
    
    // Match 'int'.
    Token* intToken = match(currentToken, INT);
    addChild(declNode, createParseTreeNode(intToken));
    
    // Match identifier and check for redeclaration.
    Token* idToken = match(currentToken, IDENTIFIER);
    if (lookupSymbol(symTable, idToken->value)) {
        printf("Semantic error: Redeclaration of variable '%s' at line %zu\n",
               idToken->value, idToken->lineNum);
        exit(1);
    }
    addChild(declNode, createParseTreeNode(idToken));
    
    // Match '='.
    Token* eqToken = match(currentToken, EQUAL);
    addChild(declNode, createParseTreeNode(eqToken));
    
    // Parse the initializing expression.
    ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
    addChild(declNode, exprNode);
    
    // Match ';'.
    Token* semicolonToken = match(currentToken, SEMICOLON);
    addChild(declNode, createParseTreeNode(semicolonToken));
    
    // Determine initial value.
    int initValue = 0;
    if (exprNode && exprNode->type == INT_L) {
        initValue = atoi(exprNode->value);
    }
    
    // Insert the variable into the symbol table with the computed initial value.
    insertSymbol(symTable, idToken->value, intToken->type, 0, NULL, initValue);
    
    return declNode;
}

// Parse an assignment: IDENTIFIER = Expression ;
ParseTreeNode* parseAssignment(Token** currentToken, SymbolTable* symTable) {
    // Create node for assignment statement.
    ParseTreeNode* assignNode = createParseTreeNode(&(Token){.value="ASSIGNMENT", .type=BEGINNING});
    
    // Match identifier and verify it exists.
    Token* idToken = match(currentToken, IDENTIFIER);
    if (!lookupSymbol(symTable, idToken->value)) {
        printf("Semantic error: Assignment to undeclared variable '%s' at line %zu\n",
               idToken->value, idToken->lineNum);
        exit(1);
    }
    addChild(assignNode, createParseTreeNode(idToken));
    
    // Match '='.
    Token* eqToken = match(currentToken, EQUAL);
    addChild(assignNode, createParseTreeNode(eqToken));
    
    // Parse the expression.
    ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
    addChild(assignNode, exprNode);
    
    // Match ';'.
    Token* semicolonToken = match(currentToken, SEMICOLON);
    addChild(assignNode, createParseTreeNode(semicolonToken));
    
    // If the expression is a literal, update the symbol's value.
    if (exprNode && exprNode->type == INT_L) {
        int newValue = atoi(exprNode->value);
        updateSymbol(symTable, idToken->value, NULL, newValue);
    }
    
    return assignNode;
}

// Parse an exit statement: exit ( Expression ) ;
ParseTreeNode* parseExitStatement(Token** currentToken, SymbolTable* symTable) {
    // Create node for exit statement.
    ParseTreeNode* exitNode = createParseTreeNode(&(Token){.value="EXIT_STATEMENT", .type=BEGINNING});
    
    // Match 'exit'.
    Token* exitToken = match(currentToken, EXIT);
    addChild(exitNode, createParseTreeNode(exitToken));
    
    // Match '('.
    Token* lpToken = match(currentToken, LPAREN);
    addChild(exitNode, createParseTreeNode(lpToken));
    
    // Parse the expression inside exit.
    ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
    addChild(exitNode, exprNode);
    
    // Match ')'.
    Token* rpToken = match(currentToken, RPAREN);
    addChild(exitNode, createParseTreeNode(rpToken));
    
    // Match ';'.
    Token* semicolonToken = match(currentToken, SEMICOLON);
    addChild(exitNode, createParseTreeNode(semicolonToken));
    
    return exitNode;
}

// Parser: Processes the entire token stream, builds the parse tree, and maintains the symbol table.
ParseTreeNode* parser(Token* tokens) {
    Token* currentToken = tokens;
    // Create the symbol table.
    SymbolTable* symTable = createSymbolTable();
    
    // Create the root node of the parse tree.
    ParseTreeNode* root = createParseTreeNode(&(Token){.value="PROGRAM", .type=BEGINNING});
    
    while (currentToken->type != END_OF_TOKENS) {
        if (currentToken->type == INT) {
            addChild(root, parseDeclaration(&currentToken, symTable));
        } else if (currentToken->type == IDENTIFIER) {
            // Look ahead to determine if it's an assignment.
            if ((currentToken + 1)->type == EQUAL) {
                addChild(root, parseAssignment(&currentToken, symTable));
            } else {
                printf("Syntax error: Unexpected identifier usage '%s' at line %zu\n",
                       currentToken->value, currentToken->lineNum);
                freeParseTree(root);
                destroySymbolTable(symTable);
                exit(1);
            }
        } else if (currentToken->type == EXIT) {
            addChild(root, parseExitStatement(&currentToken, symTable));
        } else {
            printf("Syntax error: Unexpected token '%s' at line %zu\n",
                   currentToken->value, currentToken->lineNum);
            freeParseTree(root);
            destroySymbolTable(symTable);
            exit(1);
        }
    }
    
    // Optionally, print the parse tree and symbol table for debugging.
    printf("Parse Tree:\n");
    printParseTree(root, NULL, 0);
    printf("\n");
    printSymbolTable(symTable);
    return root;
}
