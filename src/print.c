#include <stdio.h>
#include <curses.h>
#include <string.h>

#include "termconfig.h"
#include "print.h"

void
printw_spaces(WINDOW *window, int count) 
{
    for (int i = 0; i < (count < 0 ? 0 : count); i++) {
        wprintw(window, " ");
    }
}

void
wprintw_padding(WINDOW *window, int left, int right, char *s) 
{
    printw_spaces(window, left);
    wprintw(window, "%s", s);
    printw_spaces(window, right == -1 ? (termconfig.cols - strlen(s) - left) : right);
}

void
printw_padding(int left, int right, char *s) 
{
    wprintw_padding(stdscr, left, right, s);
}