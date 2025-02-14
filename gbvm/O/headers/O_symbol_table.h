#pragma once
#include "univ_hashmap.h"
#include "O_token_types.h"

typedef HashTable SymbolTable;

typedef struct {
    char *identifier;   // Name of the variable or function
    TokenType type;     // Data type (e.g., INT, etc.)
    int scopeLevel;     // Scope level (0 = global, >0 = local)
    void *memAddress;   // Memory address of the variable (if allocated)
    int value;          // Value of the variable (if known, e.g., for constant propagation)
} SymbolEntry;

SymbolTable *createSymbolTable();

void insertSymbol(SymbolTable *symTable, const char *name, TokenType type, int scope, void *memAddress, int value);

SymbolEntry *lookupSymbol(SymbolTable *symTable, const char *name);

void updateSymbol(SymbolTable *symTable, const char *name, void *memAddress, int value);

void deleteSymbol(SymbolTable *symTable, const char *name);

void destroySymbolTable(SymbolTable *symTable);

void printSymbolTable(SymbolTable *symTable);
