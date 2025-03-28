#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "univ_malloc.h"
#include "univ_fileops.h"

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

    region->last->next = part;
    region->last = part;
    cur = region->last;

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
