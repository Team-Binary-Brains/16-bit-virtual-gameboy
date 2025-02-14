#pragma once
#include "O_token_types.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"

ParseTreeNode* parser(Token*);

Token* match(Token** , TokenType);

ParseTreeNode* parseDeclaration(Token**, SymbolTable*);

ParseTreeNode* parseAssignment(Token**, SymbolTable*);

ParseTreeNode* parsePrimaryExpr(Token** currentToken, SymbolTable* symTable);

ParseTreeNode* parseMultiplicativeExpr(Token** currentToken, SymbolTable* symTable);

ParseTreeNode* parseAdditiveExpr(Token** currentToken, SymbolTable* symTable);

ParseTreeNode* parseRelationalExpr(Token** currentToken, SymbolTable* symTable);

ParseTreeNode* parseExpression(Token** currentToken, SymbolTable* symTable);

ParseTreeNode* parseExitStatement(Token**, SymbolTable*);


