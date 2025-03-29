#pragma once

#include "univ_defs.h"

#define LERP(START, END, T) (START * T + END * (1 - T))

const int YMIN = 0;
const int XMIN = 0;
int YMAX;
int XMAX;

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
}

WINDOW* createWindow(int x1, int y1, int x2, int y2)
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
    wmove(win, 1, 1);
    wrefresh(win);
    refresh();
    return win;
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