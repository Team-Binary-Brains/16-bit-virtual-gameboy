#pragma once
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "univ_strings.h"

typedef struct Partition Partition;
struct Partition {
    Partition* next;
    size_t capacity;
    size_t size;
    char buffer[];
};

#define REGION_DEFAULT_CAPACITY 65536

struct Region {
    Partition* first;
    Partition* last;
};

Partition* createPartition(size_t capacity);

void* allocateRegion(Region* region, size_t size);

int loadFileIntoRegionStr(Region* region, String filePath, String* content);

const char* convertStrToRegionCstr(Region* region, String str);

String concatRegionStr(Region* region, const char* a, const char* b);

void clearGarbage(Region* region);

void cleanRegion(Region* region);

const char* convertRegionStrtoCStr(Region* arena, String sv);
