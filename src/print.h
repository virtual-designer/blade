#ifndef __PRINT_H__
#define __PRINT_H__

#include <curses.h>

void
printw_spaces(WINDOW *window, int count);

void
printw_padding(int left, int right, char *s);

void
wprintw_padding(WINDOW *window, int left, int right, char *s);

#endif