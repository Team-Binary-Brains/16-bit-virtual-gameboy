#pragma once

#include "univ_defs.h"
#include "univ_strings.h"

#define LERP(START, END, T) (START * T + END * (1 - T))

#define MAX_WINDOW_COUNT 5

enum WindowID {
    OUTPUT,
    DETAILS,
    MEMORY,
    PROGRAM,
    INPUT
};

typedef struct {
    WINDOW* windows[MAX_WINDOW_COUNT];
    int windowCount;
} display;

void refreshWindow(WINDOW* win, String str);

display enterTUIMode(int n, String titles[]);

void exitTUIMode(display* disp);

static String WindowNames[MAX_WINDOW_COUNT] = {
    [OUTPUT] = { .data = "OUTPUT", .length = 6 },
    [DETAILS] = { .data = "DETAILS", .length = 7 },
    [MEMORY] = { .data = "MEMORY", .length = 6 },
    [PROGRAM] = { .data = "PROGRAM", .length = 8 },
    [INPUT] = { .data = "INPUT", .length = 6 }
};