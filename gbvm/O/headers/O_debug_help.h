// This Header file is only for the development purpose.
// DO NOT PUSH THIS IN PRODUCITION
#include "univ_defs.h"
#include "O_token_types.h"
#include "O_parse_tree.h"
#include "O_symbol_table.h"
#include "O_inter_codegen.h"

void __printToken(Token);

void __printTokens(Token* tokens, size_t tokensIndex);

void printParseTree(ParseTreeNode*, char*, int);

void printCurrentScope(SymbolTable* currentScope);

void printIntermediateCode(TACInstruction*);