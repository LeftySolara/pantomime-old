/*******************************************************************************
 * statusbar.h
 *******************************************************************************
 * Copyright (C) 2019-2022  Julianne Adams
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
 * @file statusbar.h
 */

#ifndef STATUSBAR_INTERNAL_H
#define STATUSBAR_INTERNAL_H

#include <ncurses.h>
#include <time.h>

#include "pantomime/mpdwrapper.h"
#include "pantomime/statusbar.h"

struct statusbar {
    WINDOW *win;
    char *modes_label;
    char *progress_label;
    char *song_label;
    char *notification;
    time_t notify_end;
};

void statusbar_initialize(struct statusbar *statusbar);
void statusbar_free(struct statusbar *statusbar);

void statusbar_draw_modes(struct statusbar *statusbar, struct mpd_status *status);
void statusbar_draw_volume(struct statusbar *statusbar, struct mpd_status *status);
void statusbar_draw_progress_bar(struct statusbar *statusbar, unsigned int time_elapsed,
                                 unsigned int song_length);
void statusbar_draw_progress_label(struct statusbar *statusbar, unsigned int time_elapsed,
                                   unsigned int song_length);
void statusbar_draw_song_label(struct statusbar *statusbar, struct mpd_song *song);
void statusbar_draw_notification(struct statusbar *statusbar);

char *statusbar_create_label_modes(char *buffer, struct mpd_status *status);
char *statusbar_create_label_progress(char *buffer, unsigned int time_elapsed,
                                      unsigned int song_length);
char *statusbar_create_label_song(char *buffer, const char *title, const char *artist);

#endif /* STATUSBAR_INTERNAL_H */
