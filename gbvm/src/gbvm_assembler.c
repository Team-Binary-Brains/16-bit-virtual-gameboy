#include "gbvm_assembler.h"

Program disassembleBytecodeIntoProgram(const char* filePath)
{
    FILE* f = openFile(filePath, "rb");
    Word m = getFileSize(f, filePath);
    Program prog;

    prog.instruction_size = sizeof(prog.instructions[0]);
    assert(m % prog.instruction_size == 0);
    prog.instruction_count = m / prog.instruction_size;
    assert(prog.instruction_count <= PROGRAM_CAPACITY);

    if (fseek(f, 0, SEEK_SET) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    prog.instruction_size = fread(prog.instructions, prog.instruction_size, prog.instruction_count, f);

    closeFile(filePath, f);

    return prog;
}

void assembleProgramIntoBytecode(const Program* prog, const char* filePath)
{
    FILE* f = openFile(filePath, "wb");

    fwrite(prog->instructions, prog->instruction_size, prog->instruction_count, f);

    closeFile(filePath, f);
}

Instruction processLine(String* line)
{
    *line = trim(*line);
    String opStr = splitStr(line, ' ');
    Opcode op = strAsOpcode(&opStr);
    if (op < PSH) {
        return (Instruction) { .type = op };
    }
    *line = ltrim(*line);
    Word operand = strToWord(splitStr(line, ' '));
    // printf("Operand :%d\n", operand);
    return (Instruction) { .type = op, .operand = operand };
}

Program parseAsmIntoProgram(String* src)
{
    Instruction ins[src->length];
    Program prog;
    prog.instruction_count = 0;
    while (src->length > 0) {
        String line = splitStr(src, '\n');
        // printf("Line : %.*s\n", (int)(line.length), line.data);
        Instruction tmp = processLine(&line);
        if (tmp.type != NOP) {
            ins[prog.instruction_count++] = tmp;
        }
    }

    memcpy(prog.instructions, ins, sizeof(ins));
    prog.instruction_size = sizeof(ins[0]);

    return prog;
}

String loadFileIntoString(const char* filePath)
{
    FILE* f = openFile(filePath, "r");
    Word m = getFileSize(f, filePath);

    char* buffer = malloc(m);

    if (buffer == NULL) {
        fileErrorDispWithExit("can't allocate memory for file", filePath);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    size_t n = fread(buffer, 1, m, f);

    closeFile(filePath, f);

    return (String) {
        .length = n,
        .data = buffer
    };
}
