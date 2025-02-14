#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wswitch"

#include "univ_defs.h"
#include "O_parser.h"
#include "O_token_types.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"

/* 
   match: Verifies that the current token matches the expected type.
   Advances the token pointer if successful; otherwise, exits with an error.
*/
Token* match(Token** currentToken, TokenType expectedType) {
    if ((*currentToken)->type == expectedType) {
        Token* matchedToken = *currentToken;
        (*currentToken)++;
        return matchedToken;
    } else {
        printf("Syntax error: Expected token type %s but found %s at line %zu\n",
               StrTokenType[expectedType], StrTokenType[(*currentToken)->type], (*currentToken)->lineNum);
        exit(1);
    }
}

/* 
   parsePrimaryExpr: Handles the most basic expressions:
     - Parenthesized Expression: "(" Expression ")"
     - Identifier: must be declared
     - Integer literal.
*/
ParseTreeNode* parsePrimaryExpr(Token** currentToken, SymbolTable* symTable) {
    if ((*currentToken)->type == LPAREN) {
        match(currentToken, LPAREN);
        ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
        match(currentToken, RPAREN);
        return exprNode;
    } else if ((*currentToken)->type == IDENTIFIER) {
        if (!lookupSymbol(symTable, (*currentToken)->value)) {
            printf("Semantic error: Undeclared variable '%s' at line %zu\n",
                   (*currentToken)->value, (*currentToken)->lineNum);
            exit(1);
        }
        return createParseTreeNode(match(currentToken, IDENTIFIER));
    } else if ((*currentToken)->type == INT_L) {
        return createParseTreeNode(match(currentToken, INT_L));
    } else {
        printf("Syntax error: Unexpected token '%s' in primary expression at line %zu\n",
               (*currentToken)->value, (*currentToken)->lineNum);
        exit(1);
    }
}

/* 
   parseMultiplicativeExpr: Handles "*", "/" and "%" operators.
*/
ParseTreeNode* parseMultiplicativeExpr(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parsePrimaryExpr(currentToken, symTable);
    while ((*currentToken)->type == STAR || (*currentToken)->type == SLASH ||
           (*currentToken)->type == MOD) {
        TokenType opType = (*currentToken)->type;
        ParseTreeNode* operatorNode = createParseTreeNode(match(currentToken, opType));
        addChild(operatorNode, left);
        addChild(operatorNode, parsePrimaryExpr(currentToken, symTable));
        left = operatorNode;
    }
    return left;
}

/* 
   parseAdditiveExpr: Handles "+" and "-" operators.
*/
ParseTreeNode* parseAdditiveExpr(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parseMultiplicativeExpr(currentToken, symTable);
    while ((*currentToken)->type == PLUS || (*currentToken)->type == MINUS) {
        TokenType opType = (*currentToken)->type;
        ParseTreeNode* operatorNode = createParseTreeNode(match(currentToken, opType));
        addChild(operatorNode, left);
        addChild(operatorNode, parseMultiplicativeExpr(currentToken, symTable));
        left = operatorNode;
    }
    return left;
}

/* 
   parseRelationalExpr: Handles relational operators:
   "==", "!=", "<", "<=", ">" and ">=".
   According to the grammar, a RelationalExpression is:
     AdditiveExpression { RelationalOperator AdditiveExpression }
   If no relational operator is found, the result is simply the AdditiveExpr.
*/
ParseTreeNode* parseRelationalExpr(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parseAdditiveExpr(currentToken, symTable);
    while ((*currentToken)->type == EQ    || (*currentToken)->type == NEQ ||
           (*currentToken)->type == LT    || (*currentToken)->type == LE   ||
           (*currentToken)->type == GT    || (*currentToken)->type == GE) {
        TokenType opType = (*currentToken)->type;
        Token* opToken = match(currentToken, opType);
        ParseTreeNode* opNode = createParseTreeNode(opToken);
        addChild(opNode, left);
        ParseTreeNode* right = parseAdditiveExpr(currentToken, symTable);
        addChild(opNode, right);
        left = opNode;
    }
    return left;
}

/* 
   parseExpression: According to our grammar,
   Expression → RelationalExpression | ArithmeticExpression.
   Since a RelationalExpression is built on top of an ArithmeticExpression,
   we simply define parseExpression as parsing a RelationalExpression.
*/
ParseTreeNode* parseExpression(Token** currentToken, SymbolTable* symTable) {
    return parseRelationalExpr(currentToken, symTable);
}

/* 
   parseDeclaration: Handles a declaration statement:
     "int" Identifier [ "=" Expression ] ";"
   An initializer is optional. If missing, the variable defaults to 0.
*/
ParseTreeNode* parseDeclaration(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* declNode = createParseTreeNode(&(Token){.value = "DECLARATION", .type = BEGINNING});
    
    Token* intToken = match(currentToken, INT);
    addChild(declNode, createParseTreeNode(intToken));
    
    Token* idToken = match(currentToken, IDENTIFIER);
    if (lookupSymbol(symTable, idToken->value)) {
        printf("Semantic error: Redeclaration of variable '%s' at line %zu\n",
               idToken->value, idToken->lineNum);
        exit(1);
    }
    addChild(declNode, createParseTreeNode(idToken));
    
    int initValue = 0;
    ParseTreeNode* exprNode = NULL;
    if ((*currentToken)->type == EQUAL) { // optional initializer
        Token* eqToken = match(currentToken, EQUAL);
        addChild(declNode, createParseTreeNode(eqToken));
        exprNode = parseExpression(currentToken, symTable);
        addChild(declNode, exprNode);
    }
    
    Token* semicolonToken = match(currentToken, SEMICOLON);
    addChild(declNode, createParseTreeNode(semicolonToken));
    
    if (exprNode && exprNode->type == INT_L) {
        initValue = atoi(exprNode->value);
    }
    insertSymbol(symTable, idToken->value, intToken->type, 0, NULL, initValue);
    
    return declNode;
}

/* 
   parseAssignment: Handles an assignment statement:
     Identifier "=" Expression ";"
*/
ParseTreeNode* parseAssignment(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* assignNode = createParseTreeNode(&(Token){.value = "ASSIGNMENT", .type = BEGINNING});
    
    Token* idToken = match(currentToken, IDENTIFIER);
    if (!lookupSymbol(symTable, idToken->value)) {
        printf("Semantic error: Assignment to undeclared variable '%s' at line %zu\n",
               idToken->value, idToken->lineNum);
        exit(1);
    }
    addChild(assignNode, createParseTreeNode(idToken));
    
    Token* eqToken = match(currentToken, EQUAL);
    addChild(assignNode, createParseTreeNode(eqToken));
    
    ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
    addChild(assignNode, exprNode);
    
    Token* semicolonToken = match(currentToken, SEMICOLON);
    addChild(assignNode, createParseTreeNode(semicolonToken));
    
    if (exprNode && exprNode->type == INT_L) {
        int newValue = atoi(exprNode->value);
        updateSymbol(symTable, idToken->value, NULL, newValue);
    }
    
    return assignNode;
}

/* 
   parseExitStatement: Handles an exit statement:
     "exit" "(" Expression ")" ";"
*/
ParseTreeNode* parseExitStatement(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* exitNode = createParseTreeNode(&(Token){.value = "EXIT_STATEMENT", .type = BEGINNING});
    
    Token* exitToken = match(currentToken, EXIT);
    addChild(exitNode, createParseTreeNode(exitToken));
    
    Token* lpToken = match(currentToken, LPAREN);
    addChild(exitNode, createParseTreeNode(lpToken));
    
    ParseTreeNode* exprNode = parseExpression(currentToken, symTable);
    addChild(exitNode, exprNode);
    
    Token* rpToken = match(currentToken, RPAREN);
    addChild(exitNode, createParseTreeNode(rpToken));
    
    Token* semicolonToken = match(currentToken, SEMICOLON);
    addChild(exitNode, createParseTreeNode(semicolonToken));
    
    return exitNode;
}

/* 
   Top-level parser: Processes the token stream, builds the parse tree,
   and maintains the symbol table.
*/
ParseTreeNode* parser(Token* tokens) {
    Token* currentToken = tokens;
    SymbolTable* symTable = createSymbolTable();
    ParseTreeNode* root = createParseTreeNode(&(Token){.value = "PROGRAM", .type = BEGINNING});
    
    while (currentToken->type != END_OF_TOKENS) {
        if (currentToken->type == INT) {
            addChild(root, parseDeclaration(&currentToken, symTable));
        } else if (currentToken->type == IDENTIFIER) {
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
    
    printParseTree(root, NULL, 0);
    printf("\n");
    printSymbolTable(symTable);
    return root;
}