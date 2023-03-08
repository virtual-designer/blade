#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>

#include "main.h"
#include "termconfig.h"
#include "print.h"

#define _GNU_SOURCE

static void
interface_atexit() 
{
	endwin();
}

void
system_error(char *custom_description, bool _strerror)
{
    endwin();
    fprintf(stderr, "%s%s%s", custom_description, _strerror ? ": " : "", _strerror ? strerror(errno) : "");
	exit(EXIT_FAILURE);
}

static void
interface_setup_color_pairs()
{
	init_pair(1, -1, COLOR_WHITE);
	init_pair(2, -1, COLOR_GREEN);
}

static void
interface_initialize() 
{
	initscr();
	use_default_colors();
	start_color();
	interface_setup_color_pairs();
	raw();
	noecho();
	keypad(stdscr, true);
	termconfig_init();
}

static void 
interface_render_statusbar()
{	
	attron(COLOR_PAIR(1));
	printw_padding(3, -1, "Welcome to FileManager -- version 1.0");
	attroff(COLOR_PAIR(1));
}

static void 
interface_render_rows()
{
	struct dirent *dirent;

	DIR *dir = opendir(".");
	int i = 0;

	while ((dirent = readdir(dir)) != NULL) 
	{
		if (i >= (termconfig.rows - 1)) {
			break;
		}

		if (termconfig.entry_selected == i) {
			attron(COLOR_PAIR(2));
		}

		printw_padding(1, -1, dirent->d_name);

		if (termconfig.entry_selected == i) {
			attroff(COLOR_PAIR(2));
		}

		i++;
	}

	termconfig.entry_count = i;

	closedir(dir);
}

static void 
interface_render()
{	
	clear();
	interface_render_statusbar();
	interface_render_rows();
	move(1, 0);
}

static bool 
interface_respond_to_keypress(int key) 
{
	int x = getcurx(stdscr), y = getcury(stdscr);
	
	switch (key) 
	{
		case 'q':
			exit(EXIT_SUCCESS);
		break;

		case KEY_DOWN:
			if (y < termconfig.rows && y < termconfig.entry_count) {
				termconfig.entry_selected++;
				interface_render();
				move(y + 1, x);
			}
			
			return false;
		break;

		case KEY_UP:
			if (y > 1) {
				termconfig.entry_selected = y - 2;
				interface_render();
				move(y - 1, x);
			}
			
			return false;
		break;

		default:
			return false;
	}

	return true;
}

static void 
interface_read_keys()
{	
	int c;

	while (true) 
	{
		c = getch();

		if (interface_respond_to_keypress(c)) {
			interface_render();
		}
	}
}

int
main(int argc, char **argv)
{
	atexit(&interface_atexit);

	interface_initialize();
	interface_render();
	interface_read_keys();

	return 0;
}
