#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "termconfig.h"
#include "main.h"

termconfig_t termconfig;

termconfig_t *
termconfig_get() 
{
    return &termconfig;
}

static void
termconfig_get_winsize(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
        system_error("ioctl failed", true);
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

void
termconfig_init()
{
    termconfig_get_winsize(&termconfig.rows, &termconfig.cols);
    termconfig.entry_count = 0;
    termconfig.entry_selected = 0;
    termconfig.filelist_window = newwin(termconfig.rows - 1, termconfig.cols - 1, 1, 0);
    termconfig.statusbar_window = newwin(1, termconfig.cols - 1, 0, 0);
}