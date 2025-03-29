#pragma once

#include "univ_defs.h"
#include "univ_strings.h"
#define MAX_WINDOW_COUNT 3
#define OUTPUT 0
#define DETAILS 1
#define MEMORY 2
#define LERP(START, END, T) (START * T + END * (1 - T))

#define YMIN 0
#define XMIN 0

typedef struct {
    WINDOW* windows[MAX_WINDOW_COUNT];
    int windowCount;
    int YMAX;
    int XMAX;
} display;

void refreshWindow(WINDOW* win, String str);

display enterTUIMode(int n, String titles[]);

void exitTUIMode(display* disp);
