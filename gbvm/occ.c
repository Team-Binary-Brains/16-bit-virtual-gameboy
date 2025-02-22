#pragma GCC diagnostic ignored "-Wswitch-enum"
#include "O_lexer.h"
#include "O_parser.h"
#include "O_codegenerator.h"
#include "univ_cmdutils.h"
#include "univ_defs.h"
#include "univ_errors.h"
#include "univ_fileops.h"
#include "O_inter_codegen.h"
#include "O_inter_code_optimizer.h"

char* inputFile = NULL;
char* outputFile = NULL;

void processFlag(char* flag, int* argc, char*** argv);

int main(int argc, char* argv[])
{
    char* progName = getNextCmdLineArg(&argc, &argv);

    while (argc > 0) {
        char* arg = getNextCmdLineArg(&argc, &argv);
        processFlag(arg, &argc, &argv);
    }

    fprintf(stdout, "\nProgram Name : %s", progName);
    fprintf(stdout, "\nInput File : %s", inputFile);
    fprintf(stdout, "\nOutput File : %s\n", outputFile);

    if (!inputFile || !outputFile) {
        displayMsgWithExit("\nMissing Files"
                           "\nUsage : gbasm -i <input file> -o <output file> -l [lang] "
                           "\tSample Command    : ./occ -i ../extras/samplePrograms/FirstProgram.osh -o ../samplePrograms/example.pblasm/n");
    }

    FILE* file = openFile(inputFile, "r");

    // Phase 1: Lexical Analysis
    Token* tokens = lexer(file, inputFile);

    // Phase 2: Syntax Analysis
    ParseTreeNode* root = parser(tokens);

    // Phase 3: Semantics Analysis
    // Basis Semantic analysis already implemented in previous phase
    // TODO: Write seperate semantic analysis along with error handler

    // Phase 4: Intermediate Code generation
    TACInstruction** tacList = generateIntermediateCode(root);

    // Phase 5: Code optimizer
    optimizeCode(tacList);

    // FILE *assembly_file = fopen("gbvmasm.asm", "r");
    // if(!assembly_file){
    //     printf("ERROR NOT KNOWN: Kya coder banega re tu\n");
    //     exit(1);
    // }

    printf("\nCompiler Ready!!!\n");

    /*
    TODO: inputting assembly file to assembler
    */
    return 0;
}

void processFlag(char* flag, int* argc, char*** argv)
{
    Option opt = flagAsOption(flag);

    switch (opt) {
    case FILE_INPUT:
        inputFile = getNextCmdLineArg(argc, argv);
        return;
    case FILE_OUTPUT:
        outputFile = getNextCmdLineArg(argc, argv);
        return;
    case ASM_LANG:
    default:
        displayMsgWithExit("Unknown option provided.");
    }
}