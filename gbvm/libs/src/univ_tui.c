#include "univ_tui.h"

WINDOW* createWindow(int x1, int y1, int x2, int y2, String str)
{
    int width = x2 - x1;
    int height = y2 - y1;

    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Error: Invalid window dimensions (%d, %d, %d, %d)\n", x1, y1, x2, y2);
        return NULL;
    }
    WINDOW* win = newwin(height, width, y1, x1);
    if (!win) {
        fprintf(stderr, "Error: Failed to create window\n");
        return NULL;
    }

    refreshWindow(win, str);

    return win;
    /*
    printw("%d %d %d %d\n", x1, y1, x2, y2);
    refresh();
    return stdscr;*/
}

void windowCreatorRecursive(display* disp, int x1, int y1, int x2, int y2, int n, String titles[])
{
    if (disp->windowCount >= MAX_WINDOW_COUNT) {
        fprintf(stderr, "Error: Exceeded MAX_WINDOW_COUNT\n");
        return;
    }
    if (n == 1) {
        disp->windows[disp->windowCount++] = createWindow(x1, y1, x2, y2, titles[disp->windowCount]);

        return;
    }
    int tmp;
    if (n % 2 == 1) {
        tmp = LERP(x1, x2, 0.6);
        disp->windows[disp->windowCount++] = createWindow(x1, y1, tmp, y2, titles[disp->windowCount]);
        windowCreatorRecursive(disp, tmp, y1, x2, y2, n - 1, titles);
        return;
    }
    tmp = LERP(y1, y2, 0.6);
    disp->windows[disp->windowCount++] = createWindow(x1, y1, x2, tmp, titles[disp->windowCount]);
    windowCreatorRecursive(disp, x1, tmp, x2, y2, n - 1, titles);

    return;
}

display CreateWindows(int n, String titles[])
{
    refresh();
    display disp;
    disp.windowCount = 0;

    for (int i = 0; i < MAX_WINDOW_COUNT; i++)
        disp.windows[i] = NULL;

    windowCreatorRecursive(&disp, XMIN, YMIN, getmaxx(stdscr), getmaxy(stdscr), n, titles);
    return disp;
}

display enterTUIMode(int n, String titles[])
{
    setlocale(LC_ALL, "");
    initscr();
    clear();
    cbreak();
    noecho();

    display disp = CreateWindows(n, titles);

    disp.YMAX = getmaxy(stdscr);
    disp.XMAX = getmaxx(stdscr);

    move(1, 1);

    return disp;
};

void exitTUIMode(display* disp)
{
    getch();

    {
        getch();
        while (disp->windowCount > 0) {
            delwin(disp->windows[disp->windowCount--]);
        }
        endwin();
    }
    endwin();
}

void refreshWindow(WINDOW* win, String str)
{
    cchar_t vline, hline, ul, ur, ll, lr;

    setcchar(&vline, L"║", 0, 0, NULL);
    setcchar(&hline, L"═", 0, 0, NULL);
    setcchar(&ul, L"╔", 0, 0, NULL);
    setcchar(&ur, L"╗", 0, 0, NULL);
    setcchar(&ll, L"╚", 0, 0, NULL);
    setcchar(&lr, L"╝", 0, 0, NULL);

    wborder_set(win, &vline, &vline, &hline, &hline, &ul, &ur, &ll, &lr);
    wmove(win, 0, (int)((getmaxx(win) - str.length - 2) / 2));
    wprintw(win, " " str_Fmt " ", str_Arg(str));
    wmove(win, 1, 1);
    wrefresh(win);
}