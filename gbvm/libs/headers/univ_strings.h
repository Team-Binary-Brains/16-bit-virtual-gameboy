#pragma once

#include "univ_defs.h"

typedef struct {
    size_t length;
    const char* data;
} String;

String ltrim(String);

String rtrim(String);

String trim(String);

String splitStr(String*, char);

int strToInt(String);

void printString(String s);