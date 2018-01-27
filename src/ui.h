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

enum panels {QUEUE, NUM_PANELS};
enum labels {CURRENT_SONG, PROGRESS, QUEUE_LEN, MODES, VOLUME, NUM_LABELS};

struct ui {
    PANEL **panels;
    WINDOW *headerbar;
    WINDOW *statusbar;

    char **labels;
    int maxx;
    int maxy;
};

struct ui *ui_init();
void ui_free(struct ui *ui);

PANEL **panels_init(struct ui *ui);
void panels_free(PANEL **panels);

void ncurses_init();

void draw_headerbar(struct ui *ui);
void draw_statusbar(struct ui *ui);
void draw_ui(struct ui *ui);

#endif
