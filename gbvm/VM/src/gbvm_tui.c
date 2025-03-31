#include "gbvm_tui.h"

static String WindowNames[MAX_WINDOW_COUNT] = {
    [OUTPUT] = { .data = "OUTPUT", .length = 6 },
    [DETAILS] = { .data = "DETAILS", .length = 7 },
    [MEMORY] = { .data = "MEMORY", .length = 6 },
    [PROGRAM] = { .data = "PROGRAM", .length = 8 },
    [INPUT] = { .data = "INPUT", .length = 6 }
};

static String Inputs[] = {
    [EXEC_SM] = { .data = "Execute the Simulated Machine Code", .length = 34 },
    [DISASSEMBLE_SM] = { .data = "Disassemble the Simulated Machine Code", .length = 38 },
    [ASSEMBLE_SASM] = { .data = "Assemble the Simulated Assembly Code into SM", .length = 44 },
    [ASSEMBLE_EXEC_SASM] = { .data = "Assemble and Execute the Simulated Assembly Code", .length = 48 },
    [COMPILE_ORIN] = { .data = "Compile the ORIN Code into SASM", .length = 33 },
    [EXIT_VM] = { .data = "Exit the Virtual Machine", .length = 24 }
};

bool createWindow(display* disp, int x1, int y1, int x2, int y2, String str)
{
    int width = x2 - x1;
    int height = y2 - y1;

    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Error: Invalid window dimensions (%d, %d, %d, %d)\n", x1, y1, x2, y2);
        return false;
    }
    WINDOW* win = newwin(height, width, y1, x1);
    if (!win) {
        fprintf(stderr, "Error: Failed to create window\n");
        return false;
    }
    disp->windows[disp->windowCount++] = win;
    refreshWindow(disp->windows[disp->windowCount - 1], str);

    return true;
}

display CreateWindows()
{
    refresh();
    display disp;
    disp.windowCount = 0;
    int xmin = 0, ymin = 0;
    int xmax = getmaxx(stdscr), ymax = getmaxy(stdscr);

    int xmid = LERP(xmin, xmax, 0.55);
    int xend = LERP(xmin, xmax, 0.13);
    int ybot = LERP(ymin, ymax, 0.3);
    int ymid = LERP(ymin, ybot, 0.7);
    createWindow(&disp, xmin, ymin, xmid, ybot, WindowNames[OUTPUT]);
    createWindow(&disp, xmin, ybot, xmax, ymax, WindowNames[INPUT]);
    createWindow(&disp, xend, ymin, xmax, ybot, WindowNames[DETAILS]);
    createWindow(&disp, xmid, ymin, xend, ymid, WindowNames[MEMORY]);
    createWindow(&disp, xmid, ymid, xend, ybot, WindowNames[PROGRAM]);

    return disp;
}

display enterTUIMode()
{
    setlocale(LC_ALL, "");
    initscr();
    clear();
    cbreak();
    // noecho();

    display disp = CreateWindows();

    move(1, 1);
    keypad(disp.windows[INPUT], true);

    return disp;
}

void exitTUIMode(display* disp)
{
    while (disp->windowCount > 0) {
        delwin(disp->windows[disp->windowCount--]);
    }
    endwin();
    exit(0);
}

void refreshWindow(WINDOW* win, String str)
{
    int x, y;
    getyx(win, y, x);
    if (x < 1)
        x = 1;
    if (y < 1)
        y = 1;

    int tmp = getmaxy(win) - 2;
    while (y > tmp) {
        wmove(win, 1, 0);
        wdeleteln(win);
        wmove(win, tmp, 0);
        wdeleteln(win);
        y -= 1;
    }

    cchar_t vline, hline, ul, ur, ll, lr;

    setcchar(&vline, L"│", 0, 0, NULL);
    setcchar(&hline, L"─", 0, 0, NULL);
    setcchar(&ul, L"╭", 0, 0, NULL);
    setcchar(&ur, L"╮", 0, 0, NULL);
    setcchar(&ll, L"╰", 0, 0, NULL);
    setcchar(&lr, L"╯", 0, 0, NULL);

    wborder_set(win, &vline, &vline, &hline, &hline, &ul, &ur, &ll, &lr);
    // wmove(win, 0, (int)((getmaxx(win) - str.length - 2) / 2));
    wmove(win, 0, 2);
    wprintw(win, " " str_Fmt " ", str_Arg(str));
    wmove(win, y, x);
    wrefresh(win);
}

void InputMenu(WINDOW* win, int* highlight, int* ch)
{
    wclear(win);
    refreshWindow(win, WindowNames[INPUT]);
    for (int i = 0; i < MAX_INPUTS; i++) {
        wmove(win, i + 2, 4);

        if (i == *highlight) {
            wattron(win, A_REVERSE);
            wprintw(win, " ▶ ");
        }
        wprintw(win, "%s", Inputs[i].data);
        wattroff(win, A_REVERSE);
    }

    *ch = wgetch(win);

    switch (*ch) {
    case KEY_UP:
        *highlight = (*highlight == 0) ? MAX_INPUTS - 1 : *highlight - 1;
        break;
    case KEY_DOWN:
        *highlight = (*highlight + 1) % MAX_INPUTS;
        break;

    default:
        break;
    }
}

void readFilePath(WINDOW* win, const char* msg, const char** filePath)
{
    char buffer[100];
    wprintw(win, "%s", msg);

    wgetnstr(win, buffer, sizeof(buffer) - 1);

    *filePath = strdup(buffer);
}

String getNameForWindow(int id)
{
    return WindowNames[id];
}

void wprintdash(WINDOW* win)
{
    wprintw(win, "\n");
    int tmp = getmaxx(win) - 1;
    for (int i = 0; i < tmp; i++) {
        wprintw(win, "─");
    }
    wprintw(win, "\n");
}