#ifndef __TERMCONFIG_H__
#define __TERMCONFIG_H__

typedef struct {
    int rows, cols;
    int entry_selected;
    int entry_count;
} termconfig_t;

extern termconfig_t termconfig;

termconfig_t *
termconfig_get();

void
termconfig_init();

#endif