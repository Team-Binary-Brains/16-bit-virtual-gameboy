#include "virex_tui.h"

static String WindowNames[MAX_WINDOW_COUNT] = {
    [OUTPUT] = { .data = "OUTPUT", .length = 6 },
    [DETAILS] = { .data = "DETAILS", .length = 7 },
    [MEMORY] = { .data = "MEMORY", .length = 6 },
    [PROGRAM] = { .data = "PROGRAM", .length = 8 },
    [INPUT] = { .data = "INPUT", .length = 6 },
    [NAME] = { .data = "VIREX", .length = 5 },
    [CREDITS] = { .data = "CREDITS", .length = 7 }
};

static String Inputs[] = {
    [EXEC_SM] = { .data = "Execute the Simulated Machine Code", .length = 34 },
    [DISASSEMBLE_SM] = { .data = "Disassemble the Simulated Machine Code", .length = 38 },
    [ASSEMBLE_SASM] = { .data = "Assemble the Simulated Assembly Code into SM", .length = 44 },
    [ASSEMBLE_EXEC_SASM] = { .data = "Assemble and Execute the Simulated Assembly Code", .length = 48 },
    [COMPILE_ORIN] = { .data = "Compile the ORIN Code into SASM", .length = 33 },
    [EXIT_VM] = { .data = "Exit the Virtual Machine", .length = 24 }
};

void initColors()
{
    start_color();
    init_color(COLOR_BLACK, 102, 106, 149);
    init_color(COLOR_RED, 999, 0, 333);
    init_color(COLOR_GREEN, 278, 921, 705);
    init_color(COLOR_YELLOW, 999, 795, 419);
    // init_color(COLOR_BLUE,);
    init_color(COLOR_MAGENTA, 615, 431, 995);
    init_color(COLOR_CYAN, 537, 866, 999);
    init_color(COLOR_WHITE, 815, 815, 815);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
}

bool createWindow(display* disp, int x1, int y1, int x2, int y2, String str, int colorPair)
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

    wbkgd(win, COLOR_PAIR(colorPair));
    disp->windows[disp->windowCount++] = win;
    refreshWindow(disp->windows[disp->windowCount - 1], str, colorPair, 5, 3);

    return true;
}

display CreateWindows()
{
    refresh();
    display disp;
    disp.windowCount = 0;
    int xmin = 0, ymin = 0;
    int xmax = getmaxx(stdscr), ymax = getmaxy(stdscr);
    /*if (xmax < 238 ){
        fprintf(stderr,"PLEASE DECREASE YOUR TERMINAL FONT SIZE");
        getch();
        exitTUIMode(&disp);
        exit(1);
    }*/

    int xsta = LERP(xmin, xmax, 0.85);
    int xmid = LERP(xsta, xmax, 0.50);
    int xmi2 = LERP(xmin, xmax, 0.68);
    int xen2 = LERP(xmin, xmax, 0.18);
    int ybot = LERP(ymin, ymax, 0.26);
    int ymid = LERP(ymin, ybot, 0.68);

    createWindow(&disp, xsta, ymin, xmid, ybot, WindowNames[OUTPUT], 1);
    createWindow(&disp, xmi2, ybot, xen2, ymax, WindowNames[INPUT], 5);
    createWindow(&disp, xmin, ymin, xsta, ybot, WindowNames[DETAILS], 4);
    createWindow(&disp, xmid, ymin, xmax, ymid, WindowNames[MEMORY], 2);
    createWindow(&disp, xmid, ymid, xmax, ybot, WindowNames[PROGRAM], 3);
    createWindow(&disp, xmin, ybot, xmi2, ymax, WindowNames[NAME], 7);
    createWindow(&disp, xen2, ybot, xmax, ymax, WindowNames[CREDITS], 7);

    return disp;
}

display enterTUIMode()
{
    setlocale(LC_ALL, "");
    initscr();
    clear();
    cbreak();
    // noecho();
    initColors();

    display disp = CreateWindows();
    wprintw(disp.windows[NAME],
        "\n    ██╗   ██╗██╗██████╗ ████████╗██╗   ██╗ █████╗ ██╗     "     //      ██╗   ██╗██╗██████╗ ███████╗██╗  ██╗"
        "\n    ██║   ██║██║██╔══██╗╚══██╔══╝██║   ██║██╔══██╗██║     "     //      ██║   ██║██║██████╔╝█████╗   ╚███╔╝ "
        "\n    ██║   ██║██║██████╔╝   ██║   ██║   ██║███████║██║     "     //      ╚██╗ ██╔╝██║██╔══██╗██╔══╝   ██╔██╗ "
        "\n    ╚██╗ ██╔╝██║██╔══██╗   ██║   ██║   ██║██╔══██║██║     "     //       ╚████╔╝ ██║██║  ██║███████╗██╔╝ ██╗"
        "\n     ╚████╔╝ ██║██║  ██║   ██║   ╚██████╔╝██║  ██║███████╗"     //        ╚═══╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝"
        "\n      ╚═══╝  ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝"
        "\n    ███████╗██╗  ██╗███████╗ ██████╗██╗   ██╗████████╗ ██████╗ ██████╗ "
        "\n    ██╔════╝╚██╗██╔╝██╔════╝██╔════╝██║   ██║╚══██╔══╝██╔═══██╗██╔══██╗"
        "\n    █████╗   ╚███╔╝ █████╗  ██║     ██║   ██║   ██║   ██║   ██║██████╔╝"
        "\n    ██╔══╝   ██╔██╗ ██╔══╝  ██║     ██║   ██║   ██║   ██║   ██║██╔══██╗"
        "\n    ███████╗██╔╝ ██╗███████╗╚██████╗╚██████╔╝   ██║   ╚██████╔╝██║  ██║"
        "\n    ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═════╝    ╚═╝    ╚═════╝ ╚═╝  ╚═╝"
        "\n                                                                       ");

    /*
    wprintw(disp.windows[CREDITS],
        "\n\n     ORIN  : OMKAR JAGTAP "
        "\n     SASM  : SOHAM METHA  "
        "\n     GBVM  : SOHAM METHA  "
        "\n     LIBS  : BOTH  ");
    */

    refreshWindow(disp.windows[NAME], WindowNames[NAME], 7, 7, 3);
    refreshWindow(disp.windows[CREDITS], WindowNames[CREDITS], 7, 7, 3);
    refreshWindow(disp.windows[MEMORY], WindowNames[MEMORY], 2, 2, 3);
    refreshWindow(disp.windows[DETAILS], WindowNames[DETAILS], 1, 1, 3);
    refreshWindow(disp.windows[PROGRAM], WindowNames[PROGRAM], 3, 2, 3);
    refreshWindow(disp.windows[OUTPUT], WindowNames[OUTPUT], 4, 5, 3);
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

void refreshWindow(WINDOW* win, String str, int contentCol, int borderCol, int titleCol)
{
    int x, y;
    getyx(win, y, x);
    if (x < 2)
        x = 2;
    if (y < 2)
        y = 2;

    int tmp = getmaxy(win) - 2;
    while (y > tmp) {
        wmove(win, 1, 0);
        wdeleteln(win);
        wmove(win, tmp, 0);
        wdeleteln(win);
        y -= 1;
    }

    wbkgd(win, COLOR_PAIR(contentCol));

    cchar_t vline, hline, ul, ur, ll, lr;

    setcchar(&vline, L"│", 0, 0, NULL);
    setcchar(&hline, L"─", 0, 0, NULL);
    setcchar(&ul, L"╭", 0, 0, NULL);
    setcchar(&ur, L"╮", 0, 0, NULL);
    setcchar(&ll, L"╰", 0, 0, NULL);
    setcchar(&lr, L"╯", 0, 0, NULL);

    // wmove(win, 0, 0);
    // wprintdash(win);
    wattron(win, COLOR_PAIR(borderCol));
    wborder_set(win, &vline, &vline, &hline, &hline, &ul, &ur, &ll, &lr);
    wattroff(win, COLOR_PAIR(borderCol));

    wattron(win, COLOR_PAIR(titleCol));
    wmove(win, 0, (int)((getmaxx(win) - str.length - 4) / 2));
    // wmove(win, 0, 2);
    wprintw(win, "❮ %s ❯", str.data);
    wattroff(win, COLOR_PAIR(titleCol));

    wmove(win, y, x);
    wrefresh(win);
}

void InputMenu(WINDOW* win, int* highlight, int* ch)
{
    wclear(win);
    refreshWindow(win, WindowNames[INPUT], 5, 5, 3);

    for (int i = 0; i < MAX_INPUTS; i++) {
        wmove(win, i + 2, 4);

        if (i == *highlight) {
            wattron(win, A_REVERSE);
            wprintw(win, " ❖  ");
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

void wprintdash(WINDOW* win, int col)
{
    wattron(win, COLOR_PAIR(col));
    int tmp = getmaxx(win) - 1;
    int i, j;
    getyx(win, j, i);
    wmove(win, j, i);
    for (; i < tmp; i++) {
        wprintw(win, "─");
    }
    wprintw(win, "\n\n");
    wattroff(win, COLOR_PAIR(col));
}