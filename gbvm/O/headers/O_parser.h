#pragma once
#include "O_lexer.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"

ParseTreeNode* parser(Token*);

Token* match(Token** , TokenType);

ParseTreeNode* parseDeclaration(Token**, SymbolTable*);

ParseTreeNode* parseAssignment(Token**, SymbolTable*);

ParseTreeNode* parseExpression(Token**, SymbolTable*);

ParseTreeNode* parseTerm(Token**, SymbolTable*);

ParseTreeNode* parseFactor(Token**, SymbolTable*);

ParseTreeNode* parseExitStatement(Token**, SymbolTable*);
