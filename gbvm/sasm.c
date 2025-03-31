#include "sasm_assembler.h"
#include "univ_strings.h"
#include "univ_malloc.h"
#include "univ_fileops.h"

const char* inputFile = NULL;
const char* outputFile = NULL;
bool disassemblyMode = false;

void processFlag(const char* program, const char* flag, int* argc, char*** argv);

int main(int argc, char** argv)
{
    const char* program = getNextCmdLineArg(&argc, &argv);

    while (argc > 0) {
        const char* flag = getNextCmdLineArg(&argc, &argv);
        processFlag(program, flag, &argc, &argv);
    }

    static Sasm sasm = { 0 };

    if (!inputFile || !outputFile) {
        displayMsgWithExit("Missing Files\n"
                           "Usage : sasm -i <input file> -o <output file> [-a | -d]\n "
                           "\t default : Assemble mode\n "
                           "\t -d : Disassemble mode\n "
                           "\tAssembly Sample Command    : ./sasm -i ./test.sasm -o ./test.sm \n"
                           "\tDisassembly Sample Command : ./sasm -i ./test.sm -o ./test.sasm -d\n");
    }

    if (!disassemblyMode) {
        parseAsmIntoProgram(&sasm, convertCstrToStr(inputFile));
        assembleProgramIntoBytecode(&sasm, outputFile);
        return 0;
    }
    FILE* f = openFile(outputFile, "wb");

    loadProgramIntoSasm(&sasm, inputFile);
    for (InstAddr i = 0; i < sasm.prog.instruction_count; ++i) {
        OpcodeDetails details = getOpcodeDetails(sasm.prog.instructions[i].type);
        fprintf(f, "%s", details.name);
        if (details.has_operand) {
            fprintf(f, " %" PRIu64, sasm.prog.instructions[i].operand.as_u64);
        }
        if (details.has_operand2) {
            fprintf(f, " %" PRIu64, sasm.prog.instructions[i].operand2.as_u64);
        }
        fprintf(f, "\n");
    }

    return 0;
}

void processFlag(const char* program, const char* flag, int* argc, char*** argv)
{
    switch (flag[1]) {
    case 'i':
        inputFile = getNextCmdLineArg(argc, argv);
        return;
    case 'o':
        outputFile = getNextCmdLineArg(argc, argv);
        return;
    case 'd':
        disassemblyMode = true;
        return;
    default:
        fprintf(stdout, "Usage: %s -i <input.sasm> -o <output.sm>\n", program);
        displayMsgWithExit("Unknown option provided.");
    }
}