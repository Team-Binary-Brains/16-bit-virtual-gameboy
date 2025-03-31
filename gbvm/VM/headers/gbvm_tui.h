#pragma once

#include "univ_defs.h"
#include "univ_strings.h"

#define LERP(START, END, T) (START * T + END * (1 - T))

/*
FONT : MONO
FONT SIZE : 10

# black
color0  #1a1b26

# red
color1  #FF0055

# green
color2  #47ebb4 <
color10 #c3e88d

# yellow
color3  #ffcb6b <
color11 #E5A100

# blue
color4  #82aaff X

# magenta
color5  #9D6EFE X

# cyan
color6  #89ddff

# white
color7  #d0d0d0
*/

enum WindowID {
    OUTPUT,
    INPUT,
    DETAILS,
    MEMORY,
    PROGRAM,
    NAME,
    CREDITS,
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

void refreshWindow(WINDOW* win, String str, int contentCol, int borderCol, int titleCol);

display enterTUIMode();

void exitTUIMode(display* disp);

void InputMenu(WINDOW* win, int* highlight, int* ch);

void readFilePath(WINDOW* win, const char* msg, const char** filePath);

String getNameForWindow(int id);

void wprintdash(WINDOW* win, int col);