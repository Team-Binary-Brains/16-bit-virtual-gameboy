#include "external_includes.h"
#include "gbvm_assembler.h"
#include "gbvm_fileops.h"

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
    if (opStr.length == 0) {
        return (Instruction) { .type = NOP };
    }
    Opcode op = strAsOpcode(&opStr);

    if (op < PSH) {
        return (Instruction) { .type = op };
    }

    *line = trim(*line);
    Word operand = strToWord(splitStr(line, ' '));
    // printf("Operand :%d\n", operand);

    return (Instruction) { .type = op, .operand = operand };
}

Program parseAsmIntoProgram(String* src)
{
    Program prog;
    prog.instruction_count = 0;
    while (src->length > 0) {
        String line = trim(splitStr(src, '\n'));
        // printf("Line : %.*s\n", (int)(line.length), line.data);

        if (line.length == 0) {
            continue;
        }

        if (*line.data == '#' || *line.data == ';') {

            debugCommentDisplay(&line);
            continue;
        }

        Instruction tmp = processLine(&line);

        if (tmp.type == NOP) {
            continue;
        }

        if (prog.instruction_count >= PROGRAM_CAPACITY) {
            displayMsgWithExit("Program LOC capacity exceeded");
        }
        prog.instructions[prog.instruction_count++] = tmp;
    }

    prog.instruction_size = sizeof(prog.instructions[0]);

    return prog;
}

String loadFileIntoString(const char* filePath)
{
    FILE* f = openFile(filePath, "r");
    Word m = getFileSize(f, filePath);

    char* buffer = malloc(m + 1);

    if (buffer == NULL) {
        fileErrorDispWithExit("can't allocate memory for file", filePath);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    size_t n = fread(buffer, 1, m, f);
    buffer[n] = '\0';

    closeFile(filePath, f);

    return (String) {
        .length = n,
        .data = buffer
    };
}
