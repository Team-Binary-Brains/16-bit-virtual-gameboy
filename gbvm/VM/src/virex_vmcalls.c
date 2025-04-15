#include "virex.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
Error vmcall_write(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    MemoryAddr addr = cpu->registers.L0.u64;
    uint64_t count = cpu->registers.QT.u64;

    if (addr >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    if (addr + count < addr || addr + count >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    wprintw(win, "%.*s", (int)count, &mem->memory[addr]);

    refreshWindow(win, getNameForWindow(OUTPUT), 1, 5, 3);

    return ERR_OK;
}

Error vmcall_alloc(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{

    cpu->registers.RF.ptr = allocateRegion(region, cpu->registers.QT.u64);

    return ERR_OK;
}

Error vmcall_free(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    clearGarbage(region);

    return ERR_OK;
}

Error vmcall_print_f64(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    wprintw(win, " %lf\n", cpu->registers.L1.f64);
    return ERR_OK;
}

Error vmcall_print_i64(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    wprintw(win, " %" PRId64 "\n", cpu->registers.L2.i64);
    return ERR_OK;
}

Error vmcall_print_u64(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    wprintw(win, " %" PRIu64 "\n", cpu->registers.L3.u64);
    return ERR_OK;
}

Error vmcall_print_ptr(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    wprintw(win, " %p\n", cpu->registers.RF.ptr);
    return ERR_OK;
}

Error vmcall_dump_memory(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    MemoryAddr addr = cpu->registers.L0.u64;
    uint64_t count = cpu->registers.QT.u64;

    if (addr >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    if (addr + count < addr || addr + count >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    for (uint64_t i = 0; i < count; ++i) {
        wprintw(win, " %02X ", mem->memory[addr + i]);
        if (i % 16 == 15) {
            wprintw(win, "\n ");
        }
        refreshWindow(win, getNameForWindow(OUTPUT), 1, 5, 3);
    }
    wprintw(win, "\n");
    refreshWindow(win, getNameForWindow(OUTPUT), 1, 5, 3);

    return ERR_OK;
}

Error vmcall_writeROM(CPU* cpu, Memory* mem, WINDOW* win, Region* region)
{
    MemoryAddr addr = cpu->registers.L0.u64;
    uint64_t count = cpu->registers.QT.u64;

    char* buffer = cpu->registers.RF.ptr;

    if (addr >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    if (addr + count < addr || addr + count >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    memcpy(buffer, &mem->memory[addr], count);

    return ERR_OK;
}