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
    DISASSEMBLE_SM,
    ASSEMBLE_SASM,
    ASSEMBLE_EXEC_SASM,
    COMPILE_ORIN,
    EXIT_VM,
    MAX_INPUTS
};

typedef struct {
    WINDOW* windows[MAX_WINDOW_COUNT];
    int windowCount;
} display;

void refreshWindow(WINDOW* win, String str);

display enterTUIMode();

void exitTUIMode(display* disp);

void InputMenu(WINDOW* win, int* highlight, int* ch);

void readFilePath(WINDOW* win, const char* msg, const char** filePath);

String getNameForWindow(int id);

void wprintdash(WINDOW* win);