/*******************************************************************************
 * ui.h
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

#ifndef UI_H
#define UI_H

#include <curses.h>
#include <panel.h>

enum ui_panel { HELP, QUEUE, LIBRARY, NUM_PANELS };

struct ui {
    PANEL **panels;
    enum ui_panel visible_panel;
};

struct ui *ui_init(void);
void ui_free(struct ui *ui);

void start_ncurses(void);
void stop_ncurses(void);

#endif
