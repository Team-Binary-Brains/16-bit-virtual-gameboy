#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "univ_malloc.h"

Partition* partitionCreate(size_t capacity)
{
    const size_t partSize = sizeof(Partition) + capacity;
    Partition* part = malloc(partSize);
    memset(part, 0, partSize);
    part->capacity = capacity;
    return part;
}

void* regionAllocAligned(Region* region, size_t size, size_t alignment)
{
    if (region->last == NULL) {
        assert(region->first == NULL);

        Partition* part = partitionCreate(
            size > REGION_DEFAULT_CAPACITY ? size : REGION_DEFAULT_CAPACITY);

        region->last = part;
        region->first = part;
    }

    if (size == 0) {
        return region->last->buffer + region->last->size;
    }

    assert((alignment & (alignment - 1)) == 0);

    Partition* cur = region->last;
    while (true) {

        char* ptr = (char*)(((uintptr_t)(cur->buffer + cur->size + (alignment - 1))) & ~(alignment - 1));
        size_t realSize = (size_t)((ptr + size) - (cur->buffer + cur->size));

        if (cur->size + realSize > cur->capacity) {
            if (cur->next) {

                cur = cur->next;
                continue;

            } else {
                size_t worstCase = size + (alignment - 1);

                Partition* part = partitionCreate(worstCase > REGION_DEFAULT_CAPACITY
                        ? worstCase
                        : REGION_DEFAULT_CAPACITY);

                region->last->next = part;
                region->last = part;
                cur = region->last;

                continue;
            }
        } else {
            memset(ptr, 0, realSize);
            cur->size += realSize;
            return ptr;
        }
    }
}

void* regionAlloc(Region* region, size_t size)
{
    return regionAllocAligned(region, size, sizeof(void*));
}

int regionSlurpFile(Region* region, String file_path, String* content)
{
    const char* file_pathCstr = regionStrToCstr(region, file_path);

    FILE* f = fopen(file_pathCstr, "rb");
    if (f == NULL) {
        return -1;
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        return -1;
    }

    long m = ftell(f);
    if (m < 0) {
        return -1;
    }

    char* buffer = regionAlloc(region, (size_t)m);
    if (buffer == NULL) {
        return -1;
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        return -1;
    }

    size_t n = fread(buffer, 1, (size_t)m, f);
    if (ferror(f)) {
        return -1;
    }

    fclose(f);

    if (content) {
        content->length = n;
        content->data = buffer;
    }

    return 0;
}

const char* regionStrToCstr(Region* region, String str)
{
    char* cstr = regionAlloc(region, str.length + 1);
    memcpy(cstr, str.data, str.length);
    cstr[str.length] = '\0';
    return cstr;
}