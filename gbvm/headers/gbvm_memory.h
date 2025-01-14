#pragma once

typedef char Byte;
typedef short int Word;
typedef int Double_Word;
typedef long long int Quad_Word;

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
    Word flags;
} CPU;

typedef struct {
    Word stack[STACK_CAPACITY];
} Memory;
