/******************************************************************************
 * statusbar.h : interface for implementing a status bar
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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <ncurses.h>
#include "mpdclient.h"

struct statusbar {
    WINDOW *win;
    char *queue_len_label;
    char *duration_label;
    char *modes_label;
};

struct statusbar *statusbar_init(struct mpdclient *mpd);
void statusbar_free(struct statusbar *statusbar);

void statusbar_set_queue_length_label(struct statusbar *statusbar, struct mpdclient *mpd);
void statusbar_set_duration_label(struct statusbar *statusbar, struct mpdclient *mpd);
void statusbar_set_modes_label(struct statusbar *statusbar, struct mpdclient *mpd);

void statusbar_draw(struct statusbar *statusbar, struct mpdclient *mpd);

#endif
