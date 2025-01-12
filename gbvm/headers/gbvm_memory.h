#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_CAPACITY 1024

typedef int8_t Byte;
typedef int16_t Word;
typedef int32_t Double_Word;
typedef int64_t Quad_Word;

typedef struct {
    Word AX;
    Word BX;
    Word CX;
    Word DX;
    Word SP;
    Word IP;
} Registers;

typedef struct {
    Registers registers;
    int halt;
} CPU;

typedef struct {
    Word stack[STACK_CAPACITY];
    Word stack_size;
} Memory;

typedef struct {
    size_t parts;
    char* data;
} String;
