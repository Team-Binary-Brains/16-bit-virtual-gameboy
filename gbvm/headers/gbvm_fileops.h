#pragma once

#include "external_includes.h"
#include "gbvm_memory.h"

FILE* openFile(const char* filePath, const char* mode);

void closeFile(const char* filePath, FILE* file);

Word getFileSize(FILE* f, const char* filePath);