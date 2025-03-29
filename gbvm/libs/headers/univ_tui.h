#pragma once

#include "univ_defs.h"
#include "univ_strings.h"
#define MAX_WINDOW_COUNT 3
#define LERP(START, END, T) (START * T + END * (1 - T))

const int YMIN = 0;
const int XMIN = 0;
int YMAX;
int XMAX;

WINDOW* windows[MAX_WINDOW_COUNT];
int windowCount = 0;

void enterTUIMode()
{
    setlocale(LC_ALL, "");
    clear();
    initscr();
    cbreak();
    noecho();

    YMAX = getmaxy(stdscr);
    XMAX = getmaxx(stdscr);
    move(1, 1);

    refresh();
};

WINDOW* createWindow(int x1, int y1, int x2, int y2, String str)
{
    WINDOW* win = newwin(y2 - y1, x2 - x1, y1, x1);
    cchar_t vline, hline, ul, ur, ll, lr;

    setcchar(&vline, L"║", 0, 0, NULL);
    setcchar(&hline, L"═", 0, 0, NULL);
    setcchar(&ul, L"╔", 0, 0, NULL);
    setcchar(&ur, L"╗", 0, 0, NULL);
    setcchar(&ll, L"╚", 0, 0, NULL);
    setcchar(&lr, L"╝", 0, 0, NULL);

    wborder_set(win, &vline, &vline, &hline, &hline, &ul, &ur, &ll, &lr);
    wmove(win, 0, (int)((x2 - x1 - str.length - 2) / 2));
    wprintw(win, " " str_Fmt " ", str_Arg(str));
    wmove(win, 1, 1);
    wrefresh(win);
    refresh();
    return win;
}

void windowCreatorRecursive(int x1, int y1, int x2, int y2, int n, String titles[])
{
    if (n == 1) {
        windows[windowCount] = createWindow(x1, y1, x2, y2, titles[windowCount++]);
        return;
    }
    int tmp;
    if (n % 2 == 1) {
        tmp = LERP(x1, x2, 0.6);
        windows[windowCount] = createWindow(x1, y1, tmp, y2, titles[windowCount++]);
        windowCreatorRecursive(tmp, y1, x2, y2, n - 1, titles);
        return;
    }
    tmp = LERP(y1, y2, 0.6);
    windows[windowCount] = createWindow(x1, y1, x2, tmp, titles[windowCount++]);
    windowCreatorRecursive(x1, tmp, x2, y2, n - 1, titles);
    return;
}

void CreateWindows(int n, String titles[])
{
    windowCreatorRecursive(XMIN, YMIN, XMAX, YMAX, n, titles);
}

void printToTUI(WINDOW* w, const char* txt)
{
    wprintw(w, "%s", txt);
}

void exitTUIMode()
{
    getch();
    endwin();
}