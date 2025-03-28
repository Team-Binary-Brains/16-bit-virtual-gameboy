#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "univ_malloc.h"

Region* regionCreate(size_t capacity)
{
    const size_t regionSize = sizeof(Region) + capacity;
    Region* region = malloc(regionSize);
    memset(region, 0, regionSize);
    region->capacity = capacity;
    return region;
}

void* partAllocAligned(Partition* part, size_t size, size_t alignment)
{
    if (part->last == NULL) {
        assert(part->first == NULL);

        Region* region = regionCreate(
            size > PART_DEFAULT_CAPACITY ? size : PART_DEFAULT_CAPACITY);

        part->last = region;
        part->first = region;
    }

    if (size == 0) {
        return part->last->buffer + part->last->size;
    }

    assert((alignment & (alignment - 1)) == 0);

    Region* cur = part->last;
    while (true) {
        uintptr_t tmp = (uintptr_t)(cur->buffer + cur->size + (alignment - 1));
        tmp = (tmp) & ~(alignment - 1);
        char* ptr = (char*)tmp;
        size_t realSize = (size_t)((ptr + size) - (cur->buffer + cur->size));

        if (cur->size + realSize <= cur->capacity) {
            memset(ptr, 0, realSize);
            cur->size += realSize;
            return ptr;
        }
        
        if (cur->next) {

            cur = cur->next;
            continue;

        }
        
        size_t worstCase = size + (alignment - 1);

        Region* region = regionCreate(worstCase > PART_DEFAULT_CAPACITY
                        ? worstCase
                        : PART_DEFAULT_CAPACITY);

        part->last->next = region;
        part->last = region;
        cur = part->last;

    }
}

void* partAlloc(Partition* part, size_t size)
{
    return partAllocAligned(part, size, sizeof(void*));
}

int partSlurpFile(Partition* part, String file_path, String* content)
{
    const char* file_pathCstr = partStrToCstr(part, file_path);

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

    char* buffer = partAlloc(part, (size_t)m);
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

const char* partStrToCstr(Partition* part, String str)
{
    char* cstr = partAlloc(part, str.length + 1);
    memcpy(cstr, str.data, str.length);
    cstr[str.length] = '\0';
    return cstr;
}
