# pragma once

#include "univ_defs.h"

// Structure for a single entry in the hash table
typedef struct Entry {
    char *key;
    char *value;
    struct Entry *next;
} Entry;

// Structure for the hash table
typedef struct {
    Entry **entries;   // Array of pointers to entries
    size_t size;       // Current size of the hash table
    size_t count;      // Number of entries in the hash table
} HashTable;

size_t hashFunc(const char *key, size_t tableSize); // Hash Function

Entry *createEntry(const char *key, const char *value); // Create Entry for Hash Table

HashTable *createHashTable(size_t size); // Create new Hash Table

void resizeHashTable(HashTable *hashtable); // Resize Hash table

void insert(HashTable *hashtable, const char *key, const char *value); // insert key value pair

char *retrieve(HashTable *hashtable, const char *key); // retrieve value from key

void deletePair(HashTable *hashtable, const char *key); // delete pair from key

void dump(HashTable *hashtable); // Print table

void destroyHashTable(HashTable *hashtable); // Delete Hash Table

