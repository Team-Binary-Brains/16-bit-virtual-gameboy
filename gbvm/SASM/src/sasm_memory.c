#include "sasm_memory.h"

void writeToMemory(Memory* memory, Word address, Byte data)
{
    memory->stack[address] = data;
}

Byte readFromMemory(Memory* memory, Word address)
{
    return memory->stack[address];
}
