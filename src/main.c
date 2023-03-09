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

argv_t argv_list = { 0, NULL };

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
	keypad(termconfig.filelist_window, true);
	box(termconfig.filelist_window, 1, 1);
}

static void 
interface_render_statusbar()
{	
	int x = getmaxx(stdscr);
	char *welcome = "Welcome to Blade File Manager -- version 1.0";
	char cwd[strlen(termconfig.cwd) + 2];

	sprintf(cwd, "[%s]", termconfig.cwd);

	wclear(termconfig.statusbar_window);
	wattron(termconfig.statusbar_window, COLOR_PAIR(1));
	wprintw_padding(termconfig.statusbar_window, 3, 0, welcome);
	wprintw_padding(termconfig.statusbar_window, 5, x - strlen(welcome) - 3, cwd);
	wattroff(termconfig.statusbar_window, COLOR_PAIR(1));
	wrefresh(termconfig.statusbar_window);
}

static void 
interface_render_rows()
{
	wclear(termconfig.filelist_window);
	struct dirent *dirent;

	DIR *dir = opendir(".");
	int i = 0;

	while ((dirent = readdir(dir)) != NULL) 
	{
		if (i >= (termconfig.rows - 1)) {
			break;
		}

		if (termconfig.entry_selected == i) {
			wattron(termconfig.filelist_window, COLOR_PAIR(2));
		}

		wprintw_padding(termconfig.filelist_window, 1, getmaxx(termconfig.filelist_window) - strlen(dirent->d_name) - 1, dirent->d_name);

		if (termconfig.entry_selected == i) {
			wattroff(termconfig.filelist_window, COLOR_PAIR(2));
		}

		i++;
	}

	termconfig.entry_count = i;

	closedir(dir);
	wrefresh(termconfig.filelist_window);
}

static void 
interface_render()
{	
	interface_render_statusbar();
	interface_render_rows();
	wmove(termconfig.filelist_window, 0, getcurx(termconfig.filelist_window));
}

static bool 
interface_respond_to_keypress(int key) 
{
	int x = getcurx(termconfig.filelist_window), y = getcury(termconfig.filelist_window);
	
	switch (key) 
	{
		case 'q':
			exit(EXIT_SUCCESS);
		break;

		case KEY_DOWN:
			if ((y + 1) < termconfig.rows && (y + 1) < termconfig.entry_count) {
				termconfig.entry_selected++;
				interface_render_rows();
				wmove(termconfig.filelist_window, y + 1, x);
			}
			
			return false;
		break;

		case KEY_UP:
			if (y > 0) {
				termconfig.entry_selected = y - 1;
				interface_render_rows();
				wmove(termconfig.filelist_window, y - 1, x);
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
		c = wgetch(termconfig.filelist_window);

		if (interface_respond_to_keypress(c)) {
			interface_render_rows();
		}
	}
}

int
main(int argc, char **argv)
{
	argv_list.argc = argc;
	argv_list.argv = argv;

	atexit(&interface_atexit);

	interface_initialize();
	interface_render();
	interface_read_keys();

	return 0;
}
