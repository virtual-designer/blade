#ifndef __TERMCONFIG_H__
#define __TERMCONFIG_H__

#include <curses.h>

typedef struct {
    int rows, cols;
    int entry_selected;
    int entry_count;
    WINDOW *filelist_window, *statusbar_window;
    char *cwd;
    int offset_y;
} termconfig_t;

extern termconfig_t termconfig;

termconfig_t *
termconfig_get();

void
termconfig_init();

#endif
