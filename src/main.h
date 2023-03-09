#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdbool.h>

typedef struct {
    int argc;
    char **argv;
} argv_t;

extern argv_t argv_list;

void
system_error(char *custom_description, bool _strerror);

#endif
