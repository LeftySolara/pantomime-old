/*******************************************************************************
 * statusbar.h
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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "../mpdwrapper/mpdwrapper.h"

#include <ncurses.h>

void draw_statusbar(WINDOW *win, struct mpdwrapper *mpd,
                    char *status_buf, char *modes_buf, char *progress_buf);

char *create_label_modes(char *buffer, struct mpd_status *status);
char *create_label_progress(char *buffer, struct mpdwrapper *mpd);
char *create_label_song(char *buffer, struct mpd_song *song);

#endif