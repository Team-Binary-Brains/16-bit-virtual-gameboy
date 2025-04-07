#include "virex.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
Error vmcall_write(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 2) {
        return ERR_STACK_UNDERFLOW;
    }

    MemoryAddr addr = mem->stack[cpu->registers.SP.u64 - 2].u64;
    uint64_t count = mem->stack[cpu->registers.SP.u64 - 1].u64;

    if (addr >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    if (addr + count < addr || addr + count >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }
    wprintw(win, "%.*s", (int)count, &mem->memory[addr]);

    refreshWindow(win, getNameForWindow(OUTPUT), 1, 5, 3);

    cpu->registers.SP.u64 -= 2;

    return ERR_OK;
}

Error vmcall_alloc(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    mem->stack[cpu->registers.SP.u64 - 1].ptr = malloc(mem->stack[cpu->registers.SP.u64 - 1].u64);

    return ERR_OK;
}

Error vmcall_free(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    free(mem->stack[cpu->registers.SP.u64 - 1].ptr);
    cpu->registers.SP.u64 -= 1;

    return ERR_OK;
}

Error vmcall_print_f64(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %lf\n", mem->stack[cpu->registers.SP.u64 - 1].f64);
    cpu->registers.SP.u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_i64(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %" PRId64 "\n", mem->stack[cpu->registers.SP.u64 - 1].i64);
    cpu->registers.SP.u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_u64(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %" PRIu64 "\n", mem->stack[cpu->registers.SP.u64 - 1].u64);
    cpu->registers.SP.u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_ptr(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %p\n", mem->stack[cpu->registers.SP.u64 - 1].ptr);
    cpu->registers.SP.u64 -= 1;
    return ERR_OK;
}

Error vmcall_dump_memory(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.u64 < 2) {
        return ERR_STACK_UNDERFLOW;
    }

    MemoryAddr addr = mem->stack[cpu->registers.SP.u64 - 2].u64;
    uint64_t count = mem->stack[cpu->registers.SP.u64 - 1].u64;

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

    cpu->registers.SP.u64 -= 2;

    return ERR_OK;
}
