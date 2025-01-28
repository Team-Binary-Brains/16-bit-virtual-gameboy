#include "sasm_memory.h"

void writeToMemory(Memory* memory, Word address, Byte data)
{
    if (address < ADDRESS_RANGE_LOW || address > ADDRESS_RANGE_HIGH) {
        displayMsgWithExit("Trying to write to memory address out of range");
    }
    memory->stack[address] = data;
}

Byte readFromMemory(Memory* memory, Word address)
{
    if (address < ADDRESS_RANGE_LOW || address > ADDRESS_RANGE_HIGH) {
        return 0x00;
    }
    return memory->stack[address];
}
