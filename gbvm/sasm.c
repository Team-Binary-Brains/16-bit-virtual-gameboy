#pragma GCC diagnostic ignored "-Wswitch-enum"

#include "sasm_assembler.h"
#include "univ_strings.h"
#include "univ_malloc.h"

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
    fprintf(stdout, "\nProgram Name : %s\n", program);
    fprintf(stdout, "Input File : %s\n", inputFile);
    fprintf(stdout, "Output File : %s\n", outputFile);
    fprintf(stdout, "Mode : %s\n\n", !disassemblyMode ? "Assembly" : "Disassembly");

    static Sasm sasm = { 0 };
    if (!disassemblyMode) {
        if (!inputFile || !outputFile) {
            displayMsgWithExit("Missing Files\n"
                               "Usage : sasm -i <input file> -o <output file> [-a | -d]\n "
                               "\t default : Assemble mode\n "
                               "\t -d : Disassemble mode\n "
                               "\tAssembly Sample Command    : ./sasm -i ./test.sasm -o ./test.sm \n"
                               "\tDisassembly Sample Command : ./sasm -i ./test.sm -d\n");
        }
        parseAsmIntoProgram(&sasm, convertCstrToStr(inputFile));
        assembleProgramIntoBytecode(&sasm, outputFile);
    } else {
        if (!inputFile) {
            displayMsgWithExit("Missing Files\n"
                               "Usage : sasm -i <input file> -o <output file> [-a | -d]\n "
                               "\t default : Assemble mode\n "
                               "\t -d : Disassemble mode\n "
                               "\tAssembly Sample Command    : ./sasm -i ./test.sasm -o ./test.sm \n"
                               "\tDisassembly Sample Command : ./sasm -i ./test.sm -d\n");
        }
        loadProgramIntoSasm(&sasm, inputFile);
        for (InstAddr i = 0; i < sasm.prog.instruction_count; ++i) {
            OpcodeDetails details = getOpcodeDetails(sasm.prog.instructions[i].type);
            printf("%s", details.name);
            if (details.has_operand) {
                printf(" %" PRIu64, sasm.prog.instructions[i].operand.as_u64);
            }
            if (details.has_operand2) {
                printf(" %" PRIu64, sasm.prog.instructions[i].operand2.as_u64);
            }
            printf("\n");
        }
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