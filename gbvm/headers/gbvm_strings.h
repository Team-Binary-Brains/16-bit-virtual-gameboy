#include "gbvm_memory.h"

typedef struct {
    size_t parts;
    const char* data;
} String;

String ltrim(String);

String rtrim(String);

String trim(String);

String splitStr(String*, char);

Word strToWord(String);

void printString(String s);