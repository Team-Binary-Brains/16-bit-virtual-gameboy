#pragma once
#include "univ_hashmap.h"
#include "O_token_types.h"

/*
    Symbol entry structure. There is no need to maintain the scope variable
    as the symbol table will have pointer to parent symbol table.
*/
typedef struct {
    char *identifier;   // Name of the variable or function
    TokenType type;     // Data type (e.g., INT, etc.)
    void *memAddress;   // Memory address of the variable (if allocated)
    int value;          // Value of the variable (if known, e.g., for constant propagation)
} SymbolEntry;

// Symbol table structure.
typedef struct SymbolTable {
    HashTable *table;           // Hash table for storing symbols
    struct SymbolTable *parent; // Pointer to the parent scope (NULL for global scope)
} SymbolTable;

// Scope management functions
SymbolTable* createSymbolTable(SymbolTable* parent);

void destroySymbolTable(SymbolTable* symTable);

SymbolEntry* lookupSymbol(SymbolTable* currentScope, const char* name);

// Symbol management functions
void insertSymbol(SymbolTable* currentScope, const char* name, TokenType type, void* memAddress, int value);

void updateSymbol(SymbolTable* currentScope, const char* name, void* memAddress, int value);

// Debugging function
void printCurrentScope(SymbolTable* currentScope);