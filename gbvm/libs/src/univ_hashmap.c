#include "univ_defs.h"
#include "univ_hashmap.h"
// Hash function to calculate the index based on the key
size_t hashFunc(const char *key, size_t tableSize) {
    size_t hashValue = 0;
    size_t i = 0;
    unsigned int keyLength = strlen(key);

    for (; i < keyLength; ++i) {
        hashValue = hashValue * 37 + key[i];
    }

    return hashValue % tableSize; // Ensure the hash value is within the table size
}

// Create a new entry (key-value pair)
Entry *createEntry(const char *key, const char *value) {
    Entry *entry = malloc(sizeof(Entry));
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);
    strcpy(entry->key, key);
    strcpy(entry->value, value);
    entry->next = NULL;
    return entry;
}

// Create a new hash table
HashTable *createHashTable(size_t size) {
    HashTable *hashtable = malloc(sizeof(HashTable));
    hashtable->size = size;
    hashtable->count = 0;
    hashtable->entries = malloc(sizeof(Entry*) * size);

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
            size_t newSlot = hashFunc(entry->key, newSize);
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
void insert(HashTable *hashtable, const char *key, const char *value) {
    // Resize if load factor exceeds threshold
    if ((double)hashtable->count / hashtable->size > LOAD_FACTOR_THRESHOLD) {
        resizeHashTable(hashtable);
    }

    size_t slot = hashFunc(key, hashtable->size);
    Entry *entry = hashtable->entries[slot];

    if (entry == NULL) {
        hashtable->entries[slot] = createEntry(key, value);
        hashtable->count++;
        return;
    }

    Entry *prev;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            strcpy(entry->value, value);
            return;
        }
        prev = entry;
        entry = prev->next;
    }

    prev->next = createEntry(key, value);
    hashtable->count++;
}

// Retrieve a value from the hash table using a key
char *retrieve(HashTable *hashtable, const char *key) {
    size_t slot = hashFunc(key, hashtable->size);
    Entry *entry = hashtable->entries[slot];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

// Delete a key-value pair from the hash table
void deletePair(HashTable *hashtable, const char *key) {
    size_t slot = hashFunc(key, hashtable->size);
    Entry *entry = hashtable->entries[slot];

    if (entry == NULL) {
        return;
    }

    Entry *prev = NULL;

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL) {
                hashtable->entries[slot] = entry->next;
            } else {
                prev->next = entry->next;
            }

            free(entry->key);
            free(entry->value);
            free(entry);
            hashtable->count--;
            return;
        }

        prev = entry;
        entry = entry->next;
    }
}

// Print the entire hash table
void dump(HashTable *hashtable) {
    for (size_t i = 0; i < hashtable->size; ++i) {
        Entry *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }

        printf("slot[%4llu]: ", i);

        while (entry != NULL) {
            printf("%s:%s ", entry->key, entry->value);
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
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }

    free(hashtable->entries);
    free(hashtable);
}
