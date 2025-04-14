#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "univ_fileops.h"
#include "univ_malloc.h"

Partition* createPartition(size_t capacity)
{
    const size_t partSize = sizeof(Partition) + capacity;
    Partition* part = malloc(partSize);
    memset(part, 0, partSize);
    part->capacity = capacity;
    return part;
}

char* insertIntoOrExpandRegion(Region* region, Partition* cur, size_t size, size_t alignedAddressMask)
{
    uintptr_t tmp = (uintptr_t)(cur->buffer + cur->size);
    tmp = (tmp + alignedAddressMask) & ~alignedAddressMask;

    char* ptr = (char*)tmp;
    size_t realSize = (ptr + size) - (cur->buffer + cur->size);

    if (cur->size + realSize <= cur->capacity) {

        memset(ptr, 0, realSize);
        cur->size += realSize;
        return ptr;
    }

    if (cur->next) {
        return insertIntoOrExpandRegion(region, cur->next, size, alignedAddressMask);
    }

    size_t worstCase = (size + alignedAddressMask) & ~alignedAddressMask;

    Partition* part = createPartition(
        worstCase > REGION_DEFAULT_CAPACITY
            ? worstCase
            : REGION_DEFAULT_CAPACITY);

    cur = region->last;
    region->last->next = part;
    region->last = part;

    return insertIntoOrExpandRegion(region, cur->next, size, alignedAddressMask);
}

void* allocateAlignedRegion(Region* region, size_t size, size_t alignment)
{
    if (region->last == NULL) {
        assert(region->first == NULL);

        Partition* part = createPartition(
            size > REGION_DEFAULT_CAPACITY ? size : REGION_DEFAULT_CAPACITY);

        region->last = part;
        region->first = part;
    }

    if (size == 0) {
        return region->last->buffer + region->last->size;
    }

    assert((alignment & (alignment - 1)) == 0);

    return insertIntoOrExpandRegion(region, region->last, size, alignment - 1);
}

void* allocateRegion(Region* region, size_t size)
{
    return allocateAlignedRegion(region, size, sizeof(void*));
}

int loadFileIntoRegionStr(Region* region, String filePath, String* content)
{
    const char* filePathCstr = convertStrToRegionCstr(region, filePath);

    FILE* f = openFile(filePathCstr, "rb");

    long m = getFileSize(f, filePath.data);

    char* buffer = allocateRegion(region, (size_t)m);
    if (buffer == NULL) {
        return -1;
    }

    size_t n = fread(buffer, 1, (size_t)m, f);
    if (ferror(f)) {
        return -1;
    }

    closeFile(f, filePath.data);

    if (content) {
        content->length = n;
        content->data = buffer;
    }

    return 0;
}

const char* convertStrToRegionCstr(Region* region, String str)
{
    char* cstr = allocateRegion(region, str.length + 1);
    memcpy(cstr, str.data, str.length);
    cstr[str.length] = '\0';
    return cstr;
}

String concatRegionStr(Region* region, const char* a, const char* b)
{
    const size_t aLen = strlen(a);
    const size_t bLen = strlen(b);
    char* buf = allocateRegion(region, aLen + bLen);
    memcpy(buf, a, aLen);
    memcpy(buf + aLen, b, bLen);
    return (String) {
        .length = aLen + bLen,
        .data = buf
    };
}

void clearGarbage(Region* region)
{
    for (Partition* part = region->first; part != NULL; part = part->next) {
        part->size = 0;
    }
    region->last = region->first;
}

void cleanRegion(Region* region)
{
    for (Partition *part = region->first, *next = NULL; part != NULL; part = next) {
        next = part->next;
        free(part);
    }
}

const char* convertRegionStrtoCStr(Region* region, String sv)
{
    char* cstr = allocateRegion(region, sv.length + 1);
    memcpy(cstr, sv.data, sv.length);
    cstr[sv.length] = '\0';
    return cstr;
}
