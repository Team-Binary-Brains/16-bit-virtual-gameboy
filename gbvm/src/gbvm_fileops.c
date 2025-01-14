#include "gbvm_fileops.h"

FILE* openFile(const char* filePath, const char* mode)
{
    FILE* f = fopen(filePath, mode);

    if (!f) {
        fileErrorDispWithExit("can't open file", filePath);
    }
    return f;
}

void closeFile(const char* filePath, FILE* file)
{
    if (ferror(file)) {
        fileErrorDispWithExit("can't write to file", filePath);
    }

    fclose(file);
}

Word getFileSize(FILE* f, const char* filePath)
{
    if (fseek(f, 0, SEEK_END) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    Word m = ftell(f);

    if (m < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }
    return m;
}