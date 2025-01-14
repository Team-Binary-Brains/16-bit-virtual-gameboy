#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_CAPACITY 1024

typedef unsigned char Byte;
typedef short int Word;
typedef unsigned int Double_Word;
typedef unsigned long long int Quad_Word;

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
