#include "sasm_memory.h"

inline void setFlag(Flags f, CPU* cpu, bool state)
{
    cpu->flags = state ? cpu->flags | f : cpu->flags & ~(f);
}

inline bool getFlag(Flags f, const CPU* cpu)
{
    return cpu->flags & f;
}
