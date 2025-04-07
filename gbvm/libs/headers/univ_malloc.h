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

Partition* createPartition(size_t capacity);

void* allocateRegion(Region* region, size_t size);

int loadFileIntoRegionStr(Region* region, String file_path, String* content);

const char* convertStrToRegionCstr(Region* region, String str);

String concatRegionStr(Region *region, const char *a, const char *b);

void clearGarbage(Region* region);

void cleanRegion(Region* region);