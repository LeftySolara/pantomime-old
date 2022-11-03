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
