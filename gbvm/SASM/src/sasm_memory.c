#include "sasm_memory.h"

inline void setFlag(Flags f, CPU* cpu, bool state)
{
    cpu->flags = state ? cpu->flags | f : cpu->flags & ~(f);
}

inline bool getFlag(Flags f, const CPU* cpu)
{
    return cpu->flags & f;
}

bool evaluateAddressingMode(Memory* mem, CPU* cpu, AddrMode mode, QuadWord* val, QuadWord* out)
{
    if (!out) {
        return false;
    }

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