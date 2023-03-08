#include <stdio.h>
#include <curses.h>
#include <string.h>

#include "termconfig.h"
#include "print.h"

void
printw_spaces(int count) 
{
    for (int i = 0; i < (count < 0 ? 0 : count); i++) {
        printw(" ");
    }
}

void
printw_padding(int left, int right, char *s) 
{
    printw_spaces(left);
    printw("%s", s);
    printw_spaces(right == -1 ? (termconfig.cols - strlen(s) - left) : right);
}