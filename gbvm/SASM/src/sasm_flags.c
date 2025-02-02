#include "univ_defs.h"
#include "sasm_memory.h"
#include "sasm_flags.h"

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