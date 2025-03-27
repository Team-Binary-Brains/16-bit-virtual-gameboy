#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "univ_strings.h"

typedef struct Region Region;

struct Region {
    Region* next;
    size_t capacity;
    size_t size;
    char buffer[];
};

Region* region_create(size_t capacity);

#define PART_DEFAULT_CAPACITY 65536

typedef struct {
    Region* first;
    Region* last;
} Partition;

Region* regionCreate(size_t capacity);

void* partAllocAligned(Partition* part, size_t size, size_t alignment);

void* partAlloc(Partition* part, size_t size);

int partSlurpFile(Partition* part, String file_path, String* content);

const char* partStrToCstr(Partition* part, String str);