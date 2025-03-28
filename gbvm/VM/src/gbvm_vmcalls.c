
#include "gbvm.h"

Error vmcall_write(CPU* cpu, Memory* mem)
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

    fwrite(&mem->memory[addr], sizeof(mem->memory[0]), count, stdout);

    cpu->registers.SP.as_u64 -= 2;

    return ERR_OK;
}

Error vmcall_alloc(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    mem->stack[cpu->registers.SP.as_u64 - 1].as_ptr = malloc(mem->stack[cpu->registers.SP.as_u64 - 1].as_u64);

    return ERR_OK;
}

Error vmcall_free(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    free(mem->stack[cpu->registers.SP.as_u64 - 1].as_ptr);
    cpu->registers.SP.as_u64 -= 1;

    return ERR_OK;
}

Error vmcall_print_f64(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    printf("%lf\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_f64);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_i64(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    printf("%" PRId64 "\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_i64);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_u64(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    printf("%" PRIu64 "\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_u64);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_print_ptr(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    printf("%p\n", mem->stack[cpu->registers.SP.as_u64 - 1].as_ptr);
    cpu->registers.SP.as_u64 -= 1;
    return ERR_OK;
}

Error vmcall_dump_memory(CPU* cpu, Memory* mem)
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
        printf("%02X ", mem->memory[addr + i]);
    }
    printf("\n");

    cpu->registers.SP.as_u64 -= 2;

    return ERR_OK;
}

Error vmcall_set_color(CPU* cpu, Memory* mem)
{
    if (cpu->registers.SP.as_u64 < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    uint64_t color = mem->stack[cpu->registers.SP.as_u64 - 1].as_u64;
    printf("\033[%ldm", color);
    cpu->registers.SP.as_u64 -= 1;

    return ERR_OK;
}