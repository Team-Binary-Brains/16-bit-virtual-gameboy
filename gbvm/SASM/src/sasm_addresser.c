#include "sasm_addresser.h"
#include "univ_strings.h"

Word evalRegister(CPU* cpu, String tmp)
{
    /*
    TODO: use a map here
    */
    if (0 == strncmp(tmp.data, "BP", 2))
        return cpu->registers.BP;

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

    trim(splitStr(s, '['));

    if (s->length <= 0)
        return s->length;

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