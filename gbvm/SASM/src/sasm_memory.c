#include "sasm_memory.h"

uint64_t evalRegister(CPU* cpu, String tmp)
{
    if (0 == strncmp(tmp.data, "EP", 2))
        return cpu->registers.EP.as_u64;

    if (0 == strncmp(tmp.data, "BX", 2))
        return cpu->registers.BX.as_u64;

    if (0 == strncmp(tmp.data, "SI", 2))
        return cpu->registers.SI.as_u64;

    if (0 == strncmp(tmp.data, "DI", 2))
        return cpu->registers.DI.as_u64;

    return convertStrToInt(tmp);
}

uint64_t resolveAddress(CPU* cpu, String* s)
{
    uint64_t res = 0;

    String ptrType = trim(splitStr(s, '['));
    if (s->length <= 0)
        return s->length;
    if (*(ptrType.data + ptrType.length - 1) == 'b') {
        printf("byte");
    } else if (*(ptrType.data + ptrType.length - 1) == 'w') {
        printf("uint64_t");
    }

    String off = splitStr(s, ']');

    String tmp = trim(splitStr(&off, ' '));

    if (0 == strncmp(tmp.data, "BP", 2)) {
        res = cpu->registers.SS.as_u64 * 16;
    } else {
        res = cpu->registers.DS.as_u64 * 16;
    }

    while (tmp.length > 0) {
        res += evalRegister(cpu, tmp);
        tmp = trim(splitStr(&off, ' '));
    }

    return res;
}

void writeToMemory(Memory* memory, MemoryAddr address, DataEntry data)
{
    memory->stack[address].as_u64 = data;
}

DataEntry readFromMemory(Memory* memory, MemoryAddr address)
{
    return memory->stack[address].as_u64;
}

inline void setFlag(Flags f, CPU* cpu, bool state)
{
    cpu->flags = state ? cpu->flags | f : cpu->flags & ~(f);
}

inline bool getFlag(Flags f, const CPU* cpu)
{
    return cpu->flags & f;
}

void checkAndSetParity(CPU* cpu, uint64_t num)
{
    bool parity = false;

    while (num) {
        parity = !parity;
        num = num & (num - 1);
    }

    cpu->flags = parity ? cpu->flags | PARITY : cpu->flags & ~(PARITY);
}

bool evaluateAddressingMode(Memory* mem, CPU* cpu, AddrMode mode, QuadWord* val, QuadWord* out)
{
    if (mode == MEM) {

        out = &(mem->stack[val->as_u64]);
        // printf("DST Data : %ld \tMode : Memory address [%ld]\n", out->as_u64, val->as_u64);

    } else if (mode == REG) {

        switch (val->as_u64) {

        case 1:
            out = &(cpu->registers.AX);
            // printf("DST Data : %ld \tMode : Register AX\n", cpu->registers.AX.as_u64);
            break;
        case 2:
            out = &(cpu->registers.BX);
            // printf("DST Data : %ld \tMode : Register BX\n", cpu->registers.BX.as_u64);
            break;
        case 3:
            out = &(cpu->registers.CX);
            // printf("DST Data : %ld \tMode : Register CX\n", cpu->registers.CX.as_u64);
            break;
        case 4:
            out = &(cpu->registers.DX);
            // printf("DST Data : %ld \tMode : Register DX\n", cpu->registers.DX.as_u64);
            break;
        }
    } else {
        out = val;
        // printf("\nSRC Data : %ld \tMode : Immediate\n", out->as_u64);
    }
    return true;
}