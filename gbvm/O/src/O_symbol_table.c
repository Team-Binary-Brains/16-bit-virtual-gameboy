#include "univ_defs.h"
#include "O_symbol_table.h"

// Helper function to free a SymbolEntry
static void symbolEntryDestroy(void *value) {
    SymbolEntry *entry = (SymbolEntry *)value;
    free(entry->identifier);
    free(entry);
}

// Create a new symbol table with an initial size and custom destroy functions.
SymbolTable *createSymbolTable() {
    // Initial table size is 100.
    return createHashTable(100, stringHashFunc, stringKeyCompare, stringKeyDestroy, symbolEntryDestroy);
}

// Insert a new symbol into the table.
void insertSymbol(SymbolTable *symTable, const char *name, TokenType type, int scope, void *memAddress, int value) {
    SymbolEntry *entry = malloc(sizeof(SymbolEntry));
    entry->identifier = strdup(name);
    entry->type = type;
    entry->scopeLevel = scope;
    entry->memAddress = memAddress;
    entry->value = value;
    insert(symTable, entry->identifier, entry);
}

// Retrieve a symbol entry from the table based on its name.
SymbolEntry *lookupSymbol(SymbolTable *symTable, const char *name) {
    return (SymbolEntry *)retrieve(symTable, name);
}

// Update a symbol's memory address and value.
void updateSymbol(SymbolTable *symTable, const char *name, void *memAddress, int value) {
    SymbolEntry *entry = lookupSymbol(symTable, name);
    if (entry) {
        entry->memAddress = memAddress;
        entry->value = value;
    }
}

// Delete a symbol entry from the table.
void deleteSymbol(SymbolTable *symTable, const char *name) {
    deletePair(symTable, name);
}

// Destroy the symbol table and free all associated memory.
void destroySymbolTable(SymbolTable *symTable) {
    destroyHashTable(symTable);
}

// A Pretty Way to print the symbol table.
void printSymbolTable(SymbolTable *symTable) {
    printf("---------- Symbol Table ----------\n");
    for (size_t i = 0; i < symTable->size; i++) {
        Entry *entry = symTable->entries[i];
        while (entry) {
            SymbolEntry *symEntry = (SymbolEntry *)entry->value;
            printf("Identifier: %-15s | Type: %-3d | Scope: %-2d | Address: %p | Value: %d\n", 
                   (char *)entry->key, symEntry->type, symEntry->scopeLevel, symEntry->memAddress, symEntry->value);
            entry = entry->next;
        }
    }
    printf("----------------------------------\n");
}
