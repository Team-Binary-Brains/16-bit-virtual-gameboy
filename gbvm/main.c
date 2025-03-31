#include "gbvm.h"
#include "gbvm_assembler.h"
#include "univ_malloc.h"
#include "gbvm_tui.h"

void processFlag(const char* program, const char* flag, int* argc, char*** argv);
void inputHandler(Vm* vm, WINDOW* win, int* highlight);
const char* inputFile = NULL;
int limit = -1;
int debug = 0;

int main(int argc, char** argv)
{
    static Vm vm = { 0 };
    loadStandardCallsIntoVm(&vm);
    vm.disp = enterTUIMode();

    const char* program = getNextCmdLineArg(&argc, &argv);

    while (argc > 0) {
        const char* flag = getNextCmdLineArg(&argc, &argv);
        processFlag(program, flag, &argc, &argv);
    }

    int ch;
    int highlight = 0;
    do {
        do {
            InputMenu(vm.disp.windows[INPUT], &highlight, &ch);
        } while (ch != '\n');

        inputHandler(&vm, vm.disp.windows[INPUT], &highlight);
        refreshWindow(vm.disp.windows[DETAILS], WindowNames[DETAILS]);
        wgetch(vm.disp.windows[INPUT]);
        vm.cpu.registers.IP.as_u64 = 0;
        setFlag(HALT, &vm.cpu, 0);

    } while (ch);
    exitTUIMode(&vm.disp);
    return 0;
}

void processFlag(const char* program, const char* flag, int* argc, char*** argv)
{

    switch (flag[1]) {
    case 'l':
        limit = atoi(getNextCmdLineArg(argc, argv));
        return;
    default:
        fprintf(stdout, "Usage: %s -i <input.sm> [-l <limit>] [-d <debug_level>]\n", program);
        displayMsgWithExit("Unknown option provided.");
    }
}

void inputHandler(Vm* vm, WINDOW* win, int* highlight)
{
    switch (*highlight) {

    case ASSEMBLE_EXEC_SASM:

    case EXEC_SM:

        wclear(win);
        refreshWindow(win, WindowNames[INPUT]);
        mvwprintw(win, 2, 4, "Enter the name of the file : ");

        char buffer[256];
        wgetnstr(win, buffer, sizeof(buffer) - 1);

        inputFile = strdup(buffer);

        loadProgramIntoVm(vm, inputFile);

        wprintw(win,
            "\n     Debug Mode?"
            "\n     0. No"
            "\n     1. Yes"
            "\n     2. Fast Debug"
            "\n     Your choice : ");
        refreshWindow(win, WindowNames[INPUT]);

        debug = wgetch(win) - '0';
        executeProgram(vm, debug, -1);

        refreshWindow(vm->disp.windows[OUTPUT], WindowNames[OUTPUT]);
        refreshWindow(vm->disp.windows[DETAILS], WindowNames[DETAILS]);
        refreshWindow(vm->disp.windows[MEMORY], WindowNames[MEMORY]);
        break;

    case ASSEMBLE_SASM:
        break;
    case COMPILE_ORIN:
    case EXIT_VM:
        exit(0);
    default:
        break;
    }
}
