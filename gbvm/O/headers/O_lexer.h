#pragma once
#include "univ_defs.h"
#include "O_token_types.h"

Token* initToken(TokenType);

void generateOpAndSepToken(char*, int*, Token*);

void generateIntLToken(char*, int*, Token*);

void generateStringLToken(char*, int*, Token*);

void generateKeywordOrIdentifier(char*, int*, Token*);

Token* lexer(FILE* file, char* inputFile);