/*******************************************************************************
 * ui.h
 *******************************************************************************
 * Copyright (C) 2019  Jalen Adams
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

#ifndef UI_H
#define UI_H

#include "../mpdwrapper/mpdwrapper.h"

#include <ncurses.h>

struct ui {
    WINDOW *statusbar;

    int maxx;
    int maxy;

    char *modes_label;
    char *progress_label;
    char *song_label;
};

void start_curses();
void end_curses();

struct ui *ui_init();
void ui_free(struct ui *ui);

void ui_draw(struct ui *ui, struct mpdwrapper *mpd);

#endif /* UI_H */
