/*******************************************************************************
 * ui.c
 *******************************************************************************
 * Copyright (C) 2017-2022  Julianne Adams
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file ui.h
 */

#include <curses.h>
#include <locale.h>
#include <panel.h>
#include <stdlib.h>

#include "ui.h"

struct ui *ui_init(void)
{
    struct ui *ui = malloc(sizeof(*ui));
    // TODO: Create panels

    start_ncurses();
    return ui;
}

void ui_free(struct ui *ui)
{
    // TODO: destroy panels
    free(ui->panels);
    free(ui);
}

/**
 * @brief Set up the NCURSES interface for the program.
 */
void start_ncurses(void)
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    refresh();
}

/**
 * @brief Stop NCURSES
 *
 */
void stop_ncurses(void)
{
    endwin();
}

/**
 * @brief Create ncurses panels with the specified width and height.
 *
 * @param num_panels The number of panels to create.
 * @param height The height of the panels.
 * @param width The width of the panels.
 *
 * @return An array of pointers to the created panels.
 *
 * The create_panels() function creates the number of specified ncurses
 * panels. These panels are where windows are drawn and are meant to be
 * viewed only one at a time. Users are able to switch between different
 * panels using keyboard commands.
 *
 * Space for the panels is allocated as if malloc() had been called.
 * To avoid memory leaks, the pointer returned from this function
 * must be passed to destroy_panels().
 */
PANEL **create_panels(int num_panels, int height, int width)
{
    PANEL **panels = malloc(sizeof(PANEL *) * num_panels);
    WINDOW *win;

    for (int i = 0; i < num_panels; ++i) {
        win = newwin(height, width, 0, 0);
        panels[i] = new_panel(win);
    }

    return panels;
}

/**
 * @brief Free memory used by panels.
 *
 * @param panels An array of PANEL pointers to free.
 * @param num_panels The size of the array.
 */
void destroy_panels(PANEL **panels, int num_panels)
{
    for (int i = 0; i < num_panels; ++i) {
        delwin(panel_window(panels[i]));
        del_panel(panels[i]);
    }
    free(panels);
}
