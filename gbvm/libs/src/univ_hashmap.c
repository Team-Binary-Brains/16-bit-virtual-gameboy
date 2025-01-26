#include "univ_defs.h"
#include "univ_hashmap.h"

// Create a new entry (key-value pair)
Entry *createEntry(void *key, void *value) {
    Entry *entry = malloc(sizeof(Entry));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    return entry;
}

// Create a new hash table
HashTable *createHashTable(size_t size, HashFunc hashFunc, KeyCompareFunc compare, KeyDestroyFunc destroyKey, ValueDestroyFunc destroyValue) {
    HashTable *hashtable = malloc(sizeof(HashTable));
    hashtable->size = size;
    hashtable->count = 0;
    hashtable->entries = malloc(sizeof(Entry*) * size);
    hashtable->hashFunc = hashFunc;
    hashtable->compare = compare;
    hashtable->destroyKey = destroyKey;
    hashtable->destroyValue = destroyValue;

    for (size_t i = 0; i < size; ++i) {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

// Resize the hash table when load factor exceeds the threshold
void resizeHashTable(HashTable *hashtable) {
    size_t newSize = hashtable->size * 2; // Double the size
    Entry **newEntries = malloc(sizeof(Entry*) * newSize);

    // Initialize the new entries array
    for (size_t i = 0; i < newSize; ++i) {
        newEntries[i] = NULL;
    }

    // Rehash all existing entries into the new array
    for (size_t i = 0; i < hashtable->size; ++i) {
        Entry *entry = hashtable->entries[i];
        while (entry) {
            size_t newSlot = hashtable->hashFunc(entry->key, newSize);
            Entry *next = entry->next;

            // Insert into the new table
            entry->next = newEntries[newSlot];
            newEntries[newSlot] = entry;

            entry = next;
        }
    }

    // Free the old entries array
    free(hashtable->entries);

    // Update the hash table with the new array and size
    hashtable->entries = newEntries;
    hashtable->size = newSize;
}

// Insert a key-value pair into the hash table
void insert(HashTable *hashtable, void *key, void *value) {
    // Resize if load factor exceeds threshold
    if ((double)hashtable->count / hashtable->size > LOAD_FACTOR_THRESHOLD) {
        resizeHashTable(hashtable);
    }

    size_t slot = hashtable->hashFunc(key, hashtable->size);
    Entry *entry = hashtable->entries[slot];

    if (entry == NULL) {
        hashtable->entries[slot] = createEntry(key, value);
        hashtable->count++;
        return;
    }

    Entry *prev;
    while (entry != NULL) {
        if (hashtable->compare(entry->key, key) == 0) {
            if (hashtable->destroyValue) {
                hashtable->destroyValue(entry->value);
            }
            entry->value = value;
            return;
        }
        prev = entry;
        entry = prev->next;
    }

    prev->next = createEntry(key, value);
    hashtable->count++;
}

// Retrieve a value from the hash table using a key
void *retrieve(HashTable *hashtable, const void *key) {
    size_t slot = hashtable->hashFunc(key, hashtable->size);
    Entry *entry = hashtable->entries[slot];
    while (entry != NULL) {
        if (hashtable->compare(entry->key, key) == 0) {
            //printf("Entry Value: %d\n", *(TokenType*)entry->value);
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

// Delete a key-value pair from the hash table
void deletePair(HashTable *hashtable, const void *key) {
    size_t slot = hashtable->hashFunc(key, hashtable->size);
    Entry *entry = hashtable->entries[slot];

    if (entry == NULL) {
        return;
    }

    Entry *prev = NULL;

    while (entry != NULL) {
        if (hashtable->compare(entry->key, key) == 0) {
            if (prev == NULL) {
                hashtable->entries[slot] = entry->next;
            } else {
                prev->next = entry->next;
            }

            if (hashtable->destroyKey) {
                hashtable->destroyKey(entry->key);
            }
            if (hashtable->destroyValue) {
                hashtable->destroyValue(entry->value);
            }
            free(entry);
            hashtable->count--;
            return;
        }

        prev = entry;
        entry = entry->next;
    }
}

// Print the entire hash table
void dump(HashTable *hashtable, void (*printKeyValue)(const void *key, const void *value)) {
    for (size_t i = 0; i < hashtable->size; ++i) {
        Entry *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }

        printf("slot[%4llu]: ", i);

        while (entry != NULL) {
            printKeyValue(entry->key, entry->value);
            entry = entry->next;
        }

        printf("\n");
    }
}

// Free all memory allocated for the hash table
void destroyHashTable(HashTable *hashtable) {
    for (size_t i = 0; i < hashtable->size; ++i) {
        Entry *entry = hashtable->entries[i];
        while (entry) {
            Entry *temp = entry;
            entry = entry->next;
            if (hashtable->destroyKey) {
                hashtable->destroyKey(temp->key);
            }
            if (hashtable->destroyValue) {
                hashtable->destroyValue(temp->value);
            }
            free(temp);
        }
    }

    free(hashtable->entries);
    free(hashtable);
}


/* -------------------- PRE DEFINED OPERATIONS FOR DIFFERENT KEY-VALUE PAIRS -------------------- */

// 1) Hash Functions
size_t stringHashFunc(const void *key, size_t tableSize) {
    const char *str = (const char *)key;
    size_t hashValue = 0;
    while (*str) {
        hashValue = hashValue * 37 + *str++;
    }
    return hashValue % tableSize;
}


// 2) Comparision Functions
int stringKeyCompare(const void *key1, const void *key2) {
    return strcmp((const char *)key1, (const char *)key2);
}


// 3) Key destruction function (for dynamically allocated keys)
void stringKeyDestroy(void *key) {
    free(key);
}


// 4) Value destruction function (for dynamically allocated values)
void intValueDestroy(void *value) {
    free(value);
}

void tokenTypeValueDestroy(void *value){
    free(value);
}


// 5) Printing Functions
void printStringInt(const void *key, const void *value) {
    printf("%s : %d ", (const char *)key, *(const int *)value);
}

