#pragma once

#include "univ_defs.h"

FILE* openFile(const char* filePath, const char* mode);

void closeFile(const char* filePath, FILE* file);

int getFileSize(FILE* f, const char* filePath);