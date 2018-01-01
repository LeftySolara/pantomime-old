/******************************************************************************
 * ui.h : user-facing elements of the program
 * ****************************************************************************
 * Copyright (C) 2017 Jalen Adams
 *
 * Authors: Jalen Adams <leftysolara@gmail.com>
 *
 * This file is part of Pantomime.
 *
 * Pantomime is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pantomime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pantomime.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <panel.h>
#include "statusbar.h"

enum panels {QUEUE, NUM_PANELS};

struct ui {
    PANEL **panels;
    struct statusbar *statusbar;
};

void ncurses_init();
PANEL **panels_init();
struct ui *ui_init();

void panels_free(PANEL **panels);
void ui_free(struct ui *ui);

void ui_draw(struct ui *ui);


#endif
