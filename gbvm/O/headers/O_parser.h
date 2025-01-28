#pragma once
#include "O_lexer.h"
#include "O_parse_tree.h"

ParseTreeNode* parser(Token*);

Token* match(Token** , TokenType);

ParseTreeNode* parseDeclaration(Token**);

ParseTreeNode* parseExpression(Token**);

ParseTreeNode* parseTerm(Token**);

ParseTreeNode* parseFactor(Token**);

ParseTreeNode* parseExitStatement(Token**);
