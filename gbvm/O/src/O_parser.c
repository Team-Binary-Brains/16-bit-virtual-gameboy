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

/* Expression Parsing */

// parsePrimaryExpr: Handles the most basic expressions:
//   "(" Expression ")" | Identifier | INT_L
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

// parseMultiplicativeExpr: Handles "*" , "/" and "%" operators.
ParseTreeNode* parseMultiplicativeExpr(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parsePrimaryExpr(currentToken, symTable);
    while ((*currentToken)->type == STAR || (*currentToken)->type == SLASH ||
           (*currentToken)->type == MOD) {
        TokenType opType = (*currentToken)->type;
        ParseTreeNode* opNode = createParseTreeNode(match(currentToken, opType));
        addChild(opNode, left);
        addChild(opNode, parsePrimaryExpr(currentToken, symTable));
        left = opNode;
    }
    return left;
}

// parseAdditiveExpr: Handles "+" and "-" operators.
ParseTreeNode* parseAdditiveExpr(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parseMultiplicativeExpr(currentToken, symTable);
    while ((*currentToken)->type == PLUS || (*currentToken)->type == MINUS) {
        TokenType opType = (*currentToken)->type;
        ParseTreeNode* opNode = createParseTreeNode(match(currentToken, opType));
        addChild(opNode, left);
        addChild(opNode, parseMultiplicativeExpr(currentToken, symTable));
        left = opNode;
    }
    return left;
}

// parseRelationalExpr: Handles relational operators: "==", "!=", "<", "<=", ">" and ">=".
ParseTreeNode* parseRelationalExpr(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* left = parseAdditiveExpr(currentToken, symTable);
    while ((*currentToken)->type == EQ || (*currentToken)->type == NEQ ||
           (*currentToken)->type == LT || (*currentToken)->type == LE ||
           (*currentToken)->type == GT || (*currentToken)->type == GE) {
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

/* Statement Parsing */

// parseDeclaration: Handles "int Identifier [ "=" Expression ] ";".
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
    if ((*currentToken)->type == EQUAL) {  // Optional initializer
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

// parseAssignment: Handles "Identifier "=" Expression ";".
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

// parseExitStatement: Handles "exit" "(" Expression ")" ";".
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

// parseIfStatement: Handles "if" "(" Expression ")" Statement [ "else" Statement ].
ParseTreeNode* parseIfStatement(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* ifNode = createParseTreeNode(&(Token){.value = "IF_STATEMENT", .type = BEGINNING});
    Token* ifToken = match(currentToken, IF);
    addChild(ifNode, createParseTreeNode(ifToken));
    match(currentToken, LPAREN);
    ParseTreeNode* condExpr = parseExpression(currentToken, symTable);
    addChild(ifNode, condExpr);
    match(currentToken, RPAREN);
    ParseTreeNode* thenStmt = parseStatement(currentToken, symTable);
    addChild(ifNode, thenStmt);
    if ((*currentToken)->type == ELSE) {
        Token* elseToken = match(currentToken, ELSE);
        addChild(ifNode, createParseTreeNode(elseToken));
        ParseTreeNode* elseStmt = parseStatement(currentToken, symTable);
        addChild(ifNode, elseStmt);
    }
    return ifNode;
}

// parseBlock: Handles "{" StatementList "}".
ParseTreeNode* parseBlock(Token** currentToken, SymbolTable* symTable) {
    ParseTreeNode* blockNode = createParseTreeNode(&(Token){.value = "BLOCK", .type = BEGINNING});
    match(currentToken, LBRACE);
    while ((*currentToken)->type != RBRACE) {
        addChild(blockNode, parseStatement(currentToken, symTable));
    }
    match(currentToken, RBRACE);
    return blockNode;
}

// parseStatement: Determines which kind of statement to parse.
ParseTreeNode* parseStatement(Token** currentToken, SymbolTable* symTable) {
    switch ((*currentToken)->type) {
        case INT:
            return parseDeclaration(currentToken, symTable);
        case IDENTIFIER:
            if (((*currentToken) + 1)->type == EQUAL) {
                return parseAssignment(currentToken, symTable);
            } else {
                printf("Syntax error: Unexpected identifier usage '%s' at line %zu\n",
                       (*currentToken)->value, (*currentToken)->lineNum);
                exit(1);
            }
        case EXIT:
            return parseExitStatement(currentToken, symTable);
        case IF:
            return parseIfStatement(currentToken, symTable);
        case LBRACE:
            return parseBlock(currentToken, symTable);
        default:
            printf("Syntax error: Unexpected token '%s' at line %zu\n",
                   (*currentToken)->value, (*currentToken)->lineNum);
            exit(1);
    }
}

// Top-level parser: Processes the token stream, builds the parse tree, and maintains the symbol table.
ParseTreeNode* parser(Token* tokens) {
    Token* currentToken = tokens;
    SymbolTable* symTable = createSymbolTable();
    ParseTreeNode* root = createParseTreeNode(&(Token){.value = "PROGRAM", .type = BEGINNING});
    while (currentToken->type != END_OF_TOKENS) {
        addChild(root, parseStatement(&currentToken, symTable));
    }
    printParseTree(root, NULL, 0);
    printf("\n");
    printSymbolTable(symTable);
    return root;
}