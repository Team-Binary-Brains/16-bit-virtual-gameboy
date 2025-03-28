#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "univ_strings.h"

typedef struct Partition Partition;
struct Partition {
    Partition* next;
    size_t capacity;
    size_t size;
    char buffer[];
};

#define REGION_DEFAULT_CAPACITY 65536

typedef struct {
    Partition* first;
    Partition* last;
} Region;

Partition* partitionCreate(size_t capacity);

void* regionAllocAligned(Region* region, size_t size, size_t alignment);

void* regionAlloc(Region* region, size_t size);

int regionSlurpFile(Region* region, String file_path, String* content);

const char* regionStrToCstr(Region* region, String str);