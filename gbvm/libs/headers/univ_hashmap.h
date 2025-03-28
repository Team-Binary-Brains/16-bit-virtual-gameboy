#pragma once

#include "univ_defs.h"

// Structure for a single entry in the hash table
typedef struct Entry {
    void* key;
    void* value;
    struct Entry* next;
} Entry;

// Function pointers for handling generic keys
typedef size_t (*HashFunc)(const void* key, size_t tableSize);
typedef int (*KeyCompareFunc)(const void* key1, const void* key2);
typedef void (*KeyDestroyFunc)(void* key);
typedef void (*ValueDestroyFunc)(void* value);
typedef void (*PrintKeyValueFunc)(const void* key, const void* value);

// Structure for the hash table
typedef struct {
    Entry** entries;                   // Array of pointers to entries
    size_t size;                       // Current size of the hash table
    size_t count;                      // Number of entries in the hash table
    HashFunc hashFunc;                 // Function to compute hash
    KeyCompareFunc compare;            // Function to compare keys
    KeyDestroyFunc destroyKey;         // Function to destroy keys
    ValueDestroyFunc destroyValue;     // Function to destroy values
} HashTable;

Entry* createEntry(void* key, void* value);     // Create Entry for Hash Table

HashTable* createHashTable(
    size_t size,
    HashFunc hashFunc,
    KeyCompareFunc compare,
    KeyDestroyFunc destroyKey,
    ValueDestroyFunc destroyValue);     // Create new Hash Table

void resizeHashTable(HashTable* hashtable);     // Resize Hash Table

void insert(HashTable* hashtable, void* key, void* value);     // Insert key-value pair

void* retrieve(HashTable* hashtable, const void* key);     // Retrieve value from key

void deletePair(HashTable* hashtable, const void* key);     // Delete pair from key

void dump(HashTable* hashtable, PrintKeyValueFunc printKeyValue);     // Print table

void destroyHashTable(HashTable* hashtable);     // Delete Hash Table

// 1) Hash Functions
size_t stringHashFunc(const void* key, size_t tableSize);

// 2) Comparision Functions
int stringKeyCompare(const void* key1, const void* key2);

// 3) Key destruction function (for dynamically allocated keys)
void stringKeyDestroy(void* key);

// 4) Value destruction function (for dynamically allocated values)
void intValueDestroy(void* value);

// 5) Printing Functions
void printStringInt(const void* key, const void* value);
