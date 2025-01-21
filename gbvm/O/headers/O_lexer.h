#pragma once
#include "univ_defs.h"

typedef enum {
    BEGINNING,
    INT,
    FLOAT,
    VOID,
    STRING,
    CHAR,
    INT_L,
    FLOAT_L,
    STR_L,
    CHAR_L,
    CONSTANT,
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COMMA,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    MOD,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NEQ,
    IDENTIFIER,
    EXIT,
    WRITE,
    END_OF_TOKENS,
    UNKNOWN,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    size_t lineNum;
} Token;

static Token SingleCharTokenMap[] = {
    { LPAREN, '(', 0 },
    { RPAREN, ')', 0 },
    { LBRACE, '{', 0 },
    { LBRACE, '{', 0 },
    { SEMICOLON, ';', 0 },
    { COMMA, ',', 0 },
    { PLUS, '+', 0 },
    { MINUS, '-', 0 },
    { STAR, '*', 0 },
    { SLASH, '/', 0 },
    { MOD, '%', 0 },
    { LT, '<', 0 },
    { GT, '>', 0 },

};

static Token TwoCharTokenMap[] = {
    { LE, '<=', 0 },
    { GE, '>=', 0 },
    { EQ, '==', 0 },
    { NEQ, '!=', 0 },
};

static Token StringTokenMap[] = {
    { INT, 'int', 0 },
    { FLOAT, 'float', 0 },
    { VOID, 'void', 0 },
    { STRING, 'float', 0 },
    { CHAR, 'char', 0 },
    { CONSTANT, 'const', 0 },
    { IF, 'if', 0 },
    { ELSE, 'else', 0 },
    { WHILE, 'while', 0 },
    { FOR, 'for', 0 },
    { RETURN, 'return', 0 },
    { EXIT, 'exit', 0 },
    { WRITE, 'cwrite', 0 },

};

void __printToken(Token);

Token* initToken(TokenType);

void generateSingleCharToken(char, Token*);

void generateTwoCharToken(char*, int*, Token*);

Token* generateIntLToken(char*, int*);

Token* generateStringLToken(char*, int*);

Token* generateKeywordOrIdentifier(char*, int*);

Token* generateSeparatorOrOperator(char*, int*, TokenType);

Token* lexer(FILE* file);