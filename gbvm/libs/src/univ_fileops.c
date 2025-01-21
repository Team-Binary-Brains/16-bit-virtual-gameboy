#include "univ_fileops.h"
#include "univ_errors.h"

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
    if (file == NULL) {
        return;
    }

    if (fclose(file) != 0) {
        fileErrorDispWithExit("can't close file", filePath);
    }
}

int getFileSize(FILE* f, const char* filePath)
{
    if (f == NULL) {
        fileErrorDispWithExit("invalid file pointer", filePath);
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    int fileSize = ftell(f);

    if (fileSize < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    rewind(f);

    return fileSize;
}