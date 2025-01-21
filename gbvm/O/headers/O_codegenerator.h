#pragma once
#include "O_lexer.h"
#include "O_parser.h"

int generateCode(Node*, char*);
void traverseTree(Node*, int is_left, FILE*);