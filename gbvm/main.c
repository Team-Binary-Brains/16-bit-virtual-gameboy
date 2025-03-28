#include "gbvm.h"
#include "gbvm_assembler.h"
#include "univ_malloc.h"

void processFlag(const char* program, const char* flag, int* argc, char*** argv);
const char* inputFile = NULL;
int limit = -1;

int main(int argc, char** argv)
{
    static Vm vm = { 0 };

    const char* program = getNextCmdLineArg(&argc, &argv);

    while (argc > 0) {
        const char* flag = getNextCmdLineArg(&argc, &argv);
        processFlag(program, flag, &argc, &argv);
    }

    if (inputFile == NULL) {
        fprintf(stdout, "Usage: %s -i <input.sm> [-l <limit>] [-h]\n", program);
        displayMsgWithExit("ERROR: input was not provided\n");
    }
    loadProgram(&vm, inputFile);
    loadStandardCalls(&vm);
    executeProgram(&vm, 0, limit);

    return 0;
}
void processFlag(const char* program, const char* flag, int* argc, char*** argv)
{

    switch (flag[1]) {
    case 'i':
        inputFile = getNextCmdLineArg(argc, argv);
        return;
    case 'l':
        limit = atoi(getNextCmdLineArg(argc, argv));
        return;
    default:
        fprintf(stdout, "Usage: %s -i <input.sm> [-l <limit>] [-h]\n", program);
        displayMsgWithExit("Unknown option provided.");
    }
}