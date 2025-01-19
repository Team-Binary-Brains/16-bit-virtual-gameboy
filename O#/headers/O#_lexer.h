#pragma once

typedef enum{
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

typedef struct{
    TokenType type;
    char *value;
    size_t lineNum;
} Token;

void __printToken(Token token);

Token *generateNumber(char *current, int *currentIndex);

Token *generateStringToken(char* current, int *currentIndex);

Token *generateKeywordOrIdentifier(char *current, int *currentIndex);

Token *generateSeperatorOrOperator(char *current, int *currentIndex, TokenType type);

Token *lexer(FILE *file);