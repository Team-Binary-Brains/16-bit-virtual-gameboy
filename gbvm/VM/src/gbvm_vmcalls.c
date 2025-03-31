
#include "gbvm.h"

Error vmcall_write(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 2) {
        return ERR_STACK_UNDERFLOW;
    }

    MemoryAddr addr = mem->stack[cpu->registers.SP.as_u64 - 2].as_u64;
    uint64_t count = mem->stack[cpu->registers.SP.as_u64 - 1].as_u64;

    if (addr >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }

    if (addr + count < addr || addr + count >= MEMORY_CAPACITY) {
        return ERR_ILLEGAL_MEMORY_ACCESS;
    }
    wprintw(win, "%.*s", (int)count, &mem->memory[addr]);

    refreshWindow(win, WindowNames[OUTPUT]);

    cpu->registers.SP.as_u64 -= 2;

    return ERR_OK;
}

Error vmcall_alloc(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    mem->stack[cpu->registers.SP.as_u64 - 1].as_ptr = malloc(mem->stack[cpu->registers.SP.as_u64 - 1].as_u64);

    return ERR_OK;
}

Error vmcall_free(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    free(mem->stack[cpu->registers.SP.as_u64 - 1].as_ptr);
    cpu->registers.SP.as_u64 -= 1;

    return ERR_OK;
}

Error vmcall_print_f64(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %lf\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_f64);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_i64(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %" PRId64 "\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_i64);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_u64(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %" PRIu64 "\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_u64);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_ptr(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    wprintw(win, " %p\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_ptr);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_dump_memory(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 2) {
        return ERR_STACK_UNDERFLOW;
    }

    MemoryAddr addr = mem->stack[cpu->registers.SP.as_u64 - 2].as_u64;
    uint64_t count = mem->stack[cpu->registers.SP.as_u64 - 1].as_u64;

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
        refreshWindow(win, WindowNames[OUTPUT]);
    }
    wprintw(win, "\n");
    refreshWindow(win, WindowNames[OUTPUT]);

    cpu->registers.SP.as_u64 -= 2;

    return ERR_OK;
}

Error vmcall_set_color(CPU* cpu, Memory* mem, WINDOW* win)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    uint64_t color = mem->stack[cpu->registers.SP.as_u64 - 1].as_u64;
    wprintw(win, " \033[%ldm", color);
    cpu->registers.SP.as_u64 -= 1;

    return ERR_OK;
}