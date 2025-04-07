#include "virex.h"
#include "univ_fileops.h"

void loadInternalCallIntoVm(Vm* vm, InternalVmCall call)
{
    assert(vm->vmCalls.internalVmCallsDefined < INTERNAL_VMCALLS_CAPACITY);
    vm->vmCalls.VmCallI[vm->vmCalls.internalVmCallsDefined++] = call;
}

void loadProgramIntoVm(Vm* vm, const char* filePath)
{
    memset(&vm->prog, 0, sizeof(vm->prog));
    FILE* f = openFile(filePath, "rb");

    Metadata meta = { 0 };

    size_t n = fread(&meta, sizeof(meta), 1, f);
    if (n < 1) {
        fprintf(stderr, "ERROR: Could not read meta data from file `%s`: %s\n",
            filePath, strerror(errno));
        exit(1);
    }

    if (meta.magic != FILE_MAGIC) {
        fprintf(stderr,
            "ERROR: %s does not appear to be a valid vm file. "
            "Unexpected magic %04X. Expected %04X.\n",
            filePath, meta.magic, FILE_MAGIC);
        exit(1);
    }

    if (meta.version != FILE_VERSION) {
        fprintf(stderr,
            "ERROR: %s: unsupported version of vm file %d. Expected version %d.\n",
            filePath, meta.version, FILE_VERSION);
        exit(1);
    }

    if (meta.programSize > PROGRAM_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: program section is too big. The file contains %" PRIu64 " program instruction. But the capacity is %" PRIu64 "\n",
            filePath, meta.programSize, (uint64_t)PROGRAM_CAPACITY);
        exit(1);
    }

    if (meta.memoryCapacity > MEMORY_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: memory section is too big. The file wants %" PRIu64 " bytes. But the capacity is %" PRIu64 " bytes\n",
            filePath, meta.memoryCapacity, (uint64_t)MEMORY_CAPACITY);
        exit(1);
    }

    if (meta.memorySize > meta.memoryCapacity) {
        fprintf(stderr,
            "ERROR: %s: memory size %" PRIu64 " is greater than declared memory capacity %" PRIu64 "\n",
            filePath, meta.memorySize, meta.memoryCapacity);
        exit(1);
    }

    if (meta.externalsSize > EXTERNAL_VMCALLS_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: external names section is too big. The file contains %" PRIu64 " external names. But the capacity is %" PRIu64 " external names\n",
            filePath, meta.externalsSize, (uint64_t)EXTERNAL_VMCALLS_CAPACITY);
        exit(1);
    }

    vm->cpu.registers.NX.as_u64 = meta.entry;

    vm->prog.instruction_count = fread(vm->prog.instructions, sizeof(vm->prog.instructions[0]), meta.programSize, f);

    if (vm->prog.instruction_count != meta.programSize) {
        fprintf(stderr, "ERROR: %s: read %" PRIu64 " program instructions, but expected %" PRIu64 "\n",
            filePath, vm->prog.instruction_count, meta.programSize);
        exit(1);
    }

    n = fread(vm->mem.memory, sizeof(vm->mem.memory[0]), meta.memorySize, f);

    if (n != meta.memorySize) {
        fprintf(stderr, "ERROR: %s: read %zd bytes of memory section, but expected %" PRIu64 " bytes.\n",
            filePath, n, meta.memorySize);
        exit(1);
    }

    vm->vmCalls.externalVmCallsDefined = fread(vm->vmCalls.VmCallE, sizeof(vm->vmCalls.VmCallE[0]), meta.externalsSize, f);
    if (vm->vmCalls.externalVmCallsDefined != meta.externalsSize) {
        fprintf(stderr, "ERROR: %s: read %zu external names, but expected %" PRIu64 "\n",
            filePath, vm->vmCalls.externalVmCallsDefined, meta.externalsSize);
        exit(1);
    }

    closeFile(f, filePath);
}

void loadStandardCallsIntoVm(Vm* vm)
{
    loadInternalCallIntoVm(vm, vmcall_alloc);           // 0
    loadInternalCallIntoVm(vm, vmcall_free);            // 1
    loadInternalCallIntoVm(vm, vmcall_print_f64);       // 2
    loadInternalCallIntoVm(vm, vmcall_print_i64);       // 3
    loadInternalCallIntoVm(vm, vmcall_print_u64);       // 4
    loadInternalCallIntoVm(vm, vmcall_print_ptr);       // 5
    loadInternalCallIntoVm(vm, vmcall_dump_memory);     // 6
    loadInternalCallIntoVm(vm, vmcall_write);           // 7
}