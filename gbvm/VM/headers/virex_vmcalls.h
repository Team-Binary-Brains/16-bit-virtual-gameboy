#pragma once
#include "sasm_memory.h"

typedef Error (*InternalVmCall)(CPU* cpu, Memory* mem, WINDOW* win,Region* region);

#define CALL_NAME_CAPACITY 256

typedef struct {
    char name[CALL_NAME_CAPACITY];
} ExternalVmCall;

typedef struct {
    InternalVmCall VmCallI[INTERNAL_VMCALLS_CAPACITY];
    size_t internalVmCallsDefined;

    ExternalVmCall VmCallE[EXTERNAL_VMCALLS_CAPACITY];
    size_t externalVmCallsDefined;
} VmCalls;

Error vmcall_write(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_alloc(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_free(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_print_f64(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_print_i64(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_print_u64(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_print_ptr(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
Error vmcall_dump_memory(CPU* cpu, Memory* mem, WINDOW* win,Region* region);
