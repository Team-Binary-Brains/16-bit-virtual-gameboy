#include "gbvm.h"

void pushCall(Vm* vm, InternalVmCall call)
{
    assert(vm->vmCalls.internalVmCallsDefined < INTERNAL_VMCALLS_CAPACITY);
    vm->vmCalls.VmCallI[vm->vmCalls.internalVmCallsDefined++] = call;
}

void loadProgram(Vm* vm, const char* file_path)
{
    memset(vm, 0, sizeof(*vm));

    FILE* f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n",
            file_path, strerror(errno));
        exit(1);
    }

    Metadata meta = { 0 };

    size_t n = fread(&meta, sizeof(meta), 1, f);
    if (n < 1) {
        fprintf(stderr, "ERROR: Could not read meta data from file `%s`: %s\n",
            file_path, strerror(errno));
        exit(1);
    }

    if (meta.magic != FILE_MAGIC) {
        fprintf(stderr,
            "ERROR: %s does not appear to be a valid vm file. "
            "Unexpected magic %04X. Expected %04X.\n",
            file_path,
            meta.magic, FILE_MAGIC);
        exit(1);
    }

    if (meta.version != FILE_VERSION) {
        fprintf(stderr,
            "ERROR: %s: unsupported version of vm file %d. Expected version %d.\n",
            file_path,
            meta.version, FILE_VERSION);
        exit(1);
    }

    if (meta.programSize > PROGRAM_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: program section is too big. The file contains %" PRIu64 " program instruction. But the capacity is %" PRIu64 "\n",
            file_path,
            meta.programSize,
            (uint64_t)PROGRAM_CAPACITY);
        exit(1);
    }

    vm->cpu.registers.IP.as_u64 = meta.entry;

    if (meta.memoryCapacity > MEMORY_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: memory section is too big. The file wants %" PRIu64 " bytes. But the capacity is %" PRIu64 " bytes\n",
            file_path,
            meta.memoryCapacity,
            (uint64_t)MEMORY_CAPACITY);
        exit(1);
    }

    if (meta.memorySize > meta.memoryCapacity) {
        fprintf(stderr,
            "ERROR: %s: memory size %" PRIu64 " is greater than declared memory capacity %" PRIu64 "\n",
            file_path,
            meta.memorySize,
            meta.memoryCapacity);
        exit(1);
    }

    if (meta.externalsSize > EXTERNAL_VMCALLS_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: external names section is too big. The file contains %" PRIu64 " external names. But the capacity is %" PRIu64 " external names\n",
            file_path,
            meta.externalsSize,
            (uint64_t)EXTERNAL_VMCALLS_CAPACITY);
        exit(1);
    }

    vm->prog.instruction_count = fread(vm->prog.instructions, sizeof(vm->prog.instructions[0]), meta.programSize, f);

    if (vm->prog.instruction_count != meta.programSize) {
        fprintf(stderr, "ERROR: %s: read %" PRIu64 " program instructions, but expected %" PRIu64 "\n",
            file_path,
            vm->prog.instruction_count,
            meta.programSize);
        exit(1);
    }

    n = fread(vm->mem.memory, sizeof(vm->mem.memory[0]), meta.memorySize, f);
    vm->mem.expectedMemorySize = meta.memorySize;

    if (n != meta.memorySize) {
        fprintf(stderr, "ERROR: %s: read %zd bytes of memory section, but expected %" PRIu64 " bytes.\n",
            file_path,
            n,
            meta.memorySize);
        exit(1);
    }

    vm->vmCalls.externalVmCallsDefined = fread(vm->vmCalls.VmCallE, sizeof(vm->vmCalls.VmCallE[0]), meta.externalsSize, f);
    if (vm->vmCalls.externalVmCallsDefined != meta.externalsSize) {
        fprintf(stderr, "ERROR: %s: read %zu external names, but expected %" PRIu64 "\n",
            file_path,
            vm->vmCalls.externalVmCallsDefined,
            meta.externalsSize);
        exit(1);
    }

    fclose(f);
}

void loadStandardCalls(Vm* vm)
{
    pushCall(vm, vmcall_alloc);           // 0
    pushCall(vm, vmcall_free);            // 1
    pushCall(vm, vmcall_print_f64);       // 2
    pushCall(vm, vmcall_print_i64);       // 3
    pushCall(vm, vmcall_print_u64);       // 4
    pushCall(vm, vmcall_print_ptr);       // 5
    pushCall(vm, vmcall_dump_memory);     // 6
    pushCall(vm, vmcall_write);           // 7
    pushCall(vm, vmcall_set_color);       // 8
}