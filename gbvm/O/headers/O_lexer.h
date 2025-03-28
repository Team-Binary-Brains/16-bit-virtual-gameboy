/**
 * @file O_lexer.h
 * @brief MISSING DOCUMENTATION.
 *
 * @author Omkar Jagtap
 * @date January 2025
 */

#pragma once
#include "univ_defs.h"
#include "O_token_types.h"

Token* initToken(TokenType);

void generateOpAndSepToken(char*, int*, Token*);

void generateIntLorFloatLToken(char*, int*, Token*);

void generateStringLToken(char*, int*, Token*);

void generateCharLToken(char* current, int* currentIndex, Token* token);

void generateKeywordOrIdentifier(char*, int*, Token*);

Token* lexer(FILE* file, char* inputFile);