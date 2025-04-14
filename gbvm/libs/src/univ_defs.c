#include "univ_defs.h"
#include "univ_strings.h"
#include "univ_malloc.h"

char* getNextCmdLineArg(int* argc, char*** argv)
{
    assert(*argc > 0);

    char* arg = **argv;

    *argc -= 1;
    *argv += 1;

    return arg;
}

bool doesFileExist(const char* filePath)
{
    struct stat statbuf = { 0 };
    return stat(filePath, &statbuf) == 0 && S_ISREG(statbuf.st_mode);
}

String appendToPath(Region* region, String base, String filePath)
{
    const String sep = convertCstrToStr("/");

    const size_t resultSize = base.length + sep.length + filePath.length;
    char* result = allocateRegion(region, resultSize);
    assert(result);

    char* append = result;

    memcpy(append, base.data, base.length);
    append += base.length;

    memcpy(append, sep.data, sep.length);
    append += sep.length;

    memcpy(append, filePath.data, filePath.length);
    append += filePath.length;

    return (String) {
        .length = resultSize,
        .data = result,
    };
}

QuadWord quadwordFromU64(uint64_t u64)
{
    return (QuadWord) { .u64 = u64 };
}

QuadWord quadwordFromI64(int64_t i64)
{
    return (QuadWord) { .i64 = i64 };
}

QuadWord quadwordFromF64(double f64)
{
    return (QuadWord) { .f64 = f64 };
}

QuadWord quadwordFromPtr(void* ptr)
{
    return (QuadWord) { .ptr = ptr };
}
