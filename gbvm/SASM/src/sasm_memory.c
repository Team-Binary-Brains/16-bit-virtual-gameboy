#include "sasm_memory.h"
#include "univ_strings.h"

Word evalRegister(CPU* cpu, String tmp)
{
    /*
    TODO: use a map here
    */
    if (0 == strncmp(tmp.data, "EP", 2))
        return cpu->registers.EP;

    if (0 == strncmp(tmp.data, "BX", 2))
        return cpu->registers.BX.full;

    if (0 == strncmp(tmp.data, "SI", 2))
        return cpu->registers.SI;

    if (0 == strncmp(tmp.data, "DI", 2))
        return cpu->registers.DI;

    return strToInt(tmp);
}

Word resolveAddress(CPU* cpu, String* s)
{
    Word res = 0;

    String ptrType = trim(splitStr(s, '['));
    if (s->length <= 0)
        return s->length;
    if (*(ptrType.data + ptrType.length - 1) == 'b') {
        printf("byte");
    } else if (*(ptrType.data + ptrType.length - 1) == 'w') {
        printf("word");
    }

    String off = splitStr(s, ']');

    String tmp = trim(splitStr(&off, ' '));

    if (0 == strncmp(tmp.data, "BP", 2)) {
        res = cpu->registers.SS * 16;
    } else {
        res = cpu->registers.DS * 16;
    }

    while (tmp.length > 0) {
        res += evalRegister(cpu, tmp);
        tmp = trim(splitStr(&off, ' '));
    }

    return res;
}

Word resolveImmediateAddress(String* s)
{
    Word res = 0;

    String ptrType = trim(splitStr(s, '['));
    if (s->length <= 0)
        return s->length;
    if (*(ptrType.data + ptrType.length - 1) == 'b') {
        printf("byte");
    } else if (*(ptrType.data + ptrType.length - 1) == 'w') {
        printf("word");
    }

    String off = splitStr(s, ']');

    String tmp = trim(splitStr(&off, ' '));

    while (tmp.length > 0) {
        res += strToInt(tmp);
        tmp = trim(splitStr(&off, ' '));
    }

    return res;
}

void writeToMemory(Memory* memory, Word address, Word data)
{
    memory->stack[address] = data;
}

Byte readFromMemory(Memory* memory, Word address)
{
    return memory->stack[address];
}

inline void setFlag(Flags f, CPU* cpu, bool state)
{
    cpu->flags = state ? cpu->flags | f : cpu->flags & ~(f);
}

inline bool getFlag(Flags f, const CPU* cpu)
{
    return cpu->flags & f;
}

void checkAndSetParity(CPU* cpu, Word num)
{
    bool parity = false;

    while (num) {
        parity = !parity;
        num = num & (num - 1);
    }

    cpu->flags = parity ? cpu->flags | PARITY : cpu->flags & ~(PARITY);
}