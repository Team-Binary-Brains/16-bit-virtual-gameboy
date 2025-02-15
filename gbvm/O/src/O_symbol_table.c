#include "univ_defs.h"
#include "O_symbol_table.h"

// Create a new symbol table, linking it to a parent scope (or NULL if global)
SymbolTable* createSymbolTable(SymbolTable* parent) {
    SymbolTable* symTable = malloc(sizeof(SymbolTable));
    symTable->table = createHashTable(100, stringHashFunc, stringKeyCompare, stringKeyDestroy, free);
    symTable->parent = parent;
    return symTable;
}

// Destroy a symbol table (but does NOT free parent scope)
void destroySymbolTable(SymbolTable* symTable) {
    destroyHashTable(symTable->table);
    free(symTable);
}

// Insert a symbol into the **current scope only**
void insertSymbol(SymbolTable* currentScope, const char* name, TokenType type, void* memAddress, int value) {
    if (!currentScope) {
        printf("Error: No active scope for insertion\n");
        exit(1);
    }

    // Check if symbol already exists in the current scope
    if (retrieve(currentScope->table, name)) {
        printf("Semantic error: Redeclaration of '%s' in the same scope\n", name);
        exit(1);
    }

    // Create symbol entry
    SymbolEntry* entry = malloc(sizeof(SymbolEntry));
    entry->identifier = strdup(name);
    entry->type = type;
    entry->memAddress = memAddress;
    entry->value = value;

    insert(currentScope->table, entry->identifier, entry);
}

// Lookup a symbol in the current scope or any parent scope (recursive search)
SymbolEntry* lookupSymbol(SymbolTable* currentScope, const char* name) {
    SymbolTable* scope = currentScope;
    while (scope) {
        SymbolEntry* entry = retrieve(scope->table, name);
        if (entry) {
            return entry;
        }
        scope = scope->parent; // Move to parent scope
    }
    return NULL; // Not found
}

// Update a symbol in the closest scope where it is declared
void updateSymbol(SymbolTable* currentScope, const char* name, void* memAddress, int value) {
    SymbolTable* scope = currentScope;
    while (scope) {
        SymbolEntry* entry = retrieve(scope->table, name);
        if (entry) {
            entry->memAddress = memAddress;
            entry->value = value;
            return;
        }
        scope = scope->parent; // Move to parent scope
    }
    printf("Semantic error: '%s' is not declared\n", name);
    exit(1);
}

// ------------------- Debugging Functions -------------------

// Print the current scope for debugging
void printCurrentScope(SymbolTable* currentScope) {
    if (!currentScope) {
        printf("\nNo active scope available\n");
        return;
    }

    printf("----------------------------------------------------------------\n");
    printf("| %-15s | %-10s | %-18s | %-8s |\n", "Identifier", "Type", "Address", "Value");
    printf("----------------------------------------------------------------\n");

    for (size_t i = 0; i < currentScope->table->size; i++) {
        Entry* entry = currentScope->table->entries[i];
        while (entry) {
            SymbolEntry* symEntry = (SymbolEntry*)entry->value;
            printf("| %-15s | %-10s | %-18p | %-8d |\n", 
                   symEntry->identifier, StrTokenType[symEntry->type], symEntry->memAddress, symEntry->value);
            entry = entry->next;
        }
    }

    printf("----------------------------------------------------------------\n");
}