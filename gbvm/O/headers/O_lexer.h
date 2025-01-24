#pragma once
#include "univ_defs.h"

typedef enum {
    BEGINNING,
    INT,
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    IDENTIFIER,
    STRING,
    COMP,
    END_OF_TOKENS,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    size_t lineNum;
} Token;

void __printToken(Token);

Token* generateNumber(char*, int*);

Token* generateStringToken(char*, int*);

Token* generateKeywordOrIdentifier(char*, int*);

Token* generateSeperatorOrOperator(char*, int*, TokenType);

Token* lexer(FILE* file);