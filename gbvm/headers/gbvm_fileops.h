#include "gbvm_errors.h"

FILE* openFile(const char* filePath, const char* mode);

void closeFile(const char* filePath, FILE* file);

Word getFileSize(FILE* f, const char* filePath);