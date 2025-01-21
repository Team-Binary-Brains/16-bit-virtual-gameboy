#include "univ_defs.h"
#include "sasm_assembler.h"
#include "univ_fileops.h"

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
    Word operand = strToInt(splitStr(line, ' '));
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

void writeProgramToFile(const Program* prog, const char* filePath)
{
    FILE* f = openFile(filePath, "w");
    for (Word i = 0; i < prog->instruction_count; i++) {
        Instruction inst = prog->instructions[i];
        String op = opcodeAsStr(&inst.type);
        fwrite(op.data, op.length, 1, f);
        if (inst.type >= PSH) {
            fwrite(" ", 1, 1, f);
            char buf[32];
            sprintf(buf, "%d", inst.operand);
            fwrite(buf, strlen(buf), 1, f);
        }
        fwrite("\n", 1, 1, f);
    }
    closeFile(filePath, f);
}

int assemblyMode(char* inputFile, char* outputFile)
{
    String src = loadFileIntoString(inputFile);

    printf("\n\n");
    printf("|--------------------------------------------------Comments detected while parsing-------------------------------------------------|\n");
    printf("|   |                                                                                                                              |\n");
    Program prog = parseAsmIntoProgram(&src);
    printf("|   |                                                                                                                              |\n");
    printf("|-------------------------------------------------------Compilation completed------------------------------------------------------|\n");
    printf("\n\n");

    assembleProgramIntoBytecode(&prog, outputFile);

    return 0;
}

int disassemblyMode(char* inputFile, char* outputFile)
{
    Program prog = disassembleBytecodeIntoProgram(inputFile);
    writeProgramToFile(&prog, outputFile);
    return 0;
}