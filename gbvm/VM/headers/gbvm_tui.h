#pragma once

#include "univ_defs.h"
#include "univ_strings.h"

#define LERP(START, END, T) (START * T + END * (1 - T))

enum WindowID {
    OUTPUT,
    INPUT,
    DETAILS,
    MEMORY,
    PROGRAM,
    MAX_WINDOW_COUNT
};

enum Inputs {
    EXEC_SM,
    ASSEMBLE_SASM,
    ASSEMBLE_EXEC_SASM,
    COMPILE_ORIN,
    EXIT_VM,
    MAX_INPUTS
};

static String WindowNames[MAX_WINDOW_COUNT] = {
    [OUTPUT] = { .data = "OUTPUT", .length = 6 },
    [DETAILS] = { .data = "DETAILS", .length = 7 },
    [MEMORY] = { .data = "MEMORY", .length = 6 },
    [PROGRAM] = { .data = "PROGRAM", .length = 8 },
    [INPUT] = { .data = "INPUT", .length = 6 }
};

static String Inputs[] = {
    [EXEC_SM] = { .data = "Execute the Simulated Machine Code", .length = 34 },
    [ASSEMBLE_SASM] = { .data = "Assemble the Simulated Assembly Code into SM", .length = 44 },
    [ASSEMBLE_EXEC_SASM] = { .data = "Assemble and Execute the Simulated Assembly Code", .length = 48 },
    [COMPILE_ORIN] = { .data = "Compile the ORIN Assembly Code into SASM", .length = 40 },
    [EXIT_VM] = { .data = "Exit the Virtual Machine", .length = 24 }
};

typedef struct {
    WINDOW* windows[MAX_WINDOW_COUNT];
    int windowCount;
} display;

void refreshWindow(WINDOW* win, String str);

display enterTUIMode();

void exitTUIMode(display* disp);

void InputMenu(WINDOW* win, int* highlight, int* ch);