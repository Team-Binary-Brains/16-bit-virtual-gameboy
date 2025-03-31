#include "gbvm.h"
#include "gbvm_assembler.h"
#include "univ_malloc.h"
#include "univ_tui.h"

void processFlag(const char* program, const char* flag, int* argc, char*** argv);
const char* inputFile = NULL;
int limit = -1;
int debug = 0;

int main(int argc, char** argv)
{
    static Vm vm = { 0 };
    vm.disp = enterTUIMode();

    const char* program = getNextCmdLineArg(&argc, &argv);

    while (argc > 0) {
        const char* flag = getNextCmdLineArg(&argc, &argv);
        processFlag(program, flag, &argc, &argv);
    }

    if (inputFile == NULL) {
        fprintf(stdout, "Usage: %s -i <input.sm> [-l <limit>] [-d <debug_level>]\n", program);
        displayMsgWithExit("ERROR: input was not provided\n");
    }

    int ch;
    int highlight = 0;
    do {

        for (size_t i = 0; i < MAX_INPUTS; i++) {
            if (i == highlight) {
                wattron(vm.disp.windows[INPUT], A_REVERSE);
            }
            mvwprintw(vm.disp.windows[INPUT], i + 2, 4, Inputs[i].data);
            wattroff(vm.disp.windows[INPUT], A_REVERSE);
        }

        ch = wgetch(vm.disp.windows[INPUT]);

        switch (ch) {
        case KEY_UP:
            highlight = (highlight == 0) ? MAX_INPUTS - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % MAX_INPUTS;
            break;

        default:
            break;
        }

    } while (ch);

    loadProgramIntoVm(&vm, inputFile);
    loadStandardCallsIntoVm(&vm);

    executeProgram(&vm, debug, limit);

    exitTUIMode(&vm.disp);
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
    case 'd':
        debug = atoi(getNextCmdLineArg(argc, argv));
        return;
    default:
        fprintf(stdout, "Usage: %s -i <input.sm> [-l <limit>] [-d <debug_level>]\n", program);
        displayMsgWithExit("Unknown option provided.");
    }
}