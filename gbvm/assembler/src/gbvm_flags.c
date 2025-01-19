#include "global_definitions.h"
#include "gbvm_memory.h"
#include "gbvm_flags.h"

void setHalt(CPU* cpu, bool halt)
{
    cpu->flags = halt ? cpu->flags | HALT : cpu->flags & !(HALT);
}

bool getHalt(const CPU* cpu)
{
    return cpu->flags & HALT;
}

void setSign(CPU* cpu, bool sign)
{
    cpu->flags = sign ? cpu->flags | SIGN : cpu->flags & !(SIGN);
}

bool getSign(const CPU* cpu)
{
    return cpu->flags & SIGN;
}

void setOverflow(CPU* cpu, bool overflow)
{
    cpu->flags = overflow ? cpu->flags | OVERFLOW : cpu->flags & !(OVERFLOW);
}

bool getOverflow(const CPU* cpu)
{
    return cpu->flags & OVERFLOW;
}

void setCarry(CPU* cpu, bool carry)
{
    cpu->flags = carry ? cpu->flags | CARRY : cpu->flags & !(CARRY);
}

bool getCarry(const CPU* cpu)
{
    return cpu->flags & CARRY;
}

void setBorrow(CPU* cpu, bool borrow)
{
    cpu->flags = borrow ? cpu->flags | BORROW : cpu->flags & !(BORROW);
}

bool getBorrow(const CPU* cpu)
{
    return cpu->flags & BORROW;
}

void setParity(CPU* cpu, bool parity)
{
    cpu->flags = parity ? cpu->flags | PARITY : cpu->flags & !(PARITY);
}

bool getParity(const CPU* cpu)
{
    return cpu->flags & PARITY;
}

void setZero(CPU* cpu, bool zero)
{
    cpu->flags = zero ? cpu->flags | ZERO : cpu->flags & !(ZERO);
}

bool getZero(const CPU* cpu)
{
    return cpu->flags & ZERO;
}
