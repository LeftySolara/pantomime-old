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
#include <time.h>

struct status_bar {
    WINDOW *win;
    char *modes_label;
    char *progress_label;
    char *song_label;
    char *notification;
    time_t notify_end;
};

struct status_bar *status_bar_init();
void status_bar_free(struct status_bar *bar);

void draw_statusbar(struct status_bar *status_bar, struct mpdwrapper *mpd);
void draw_modes(struct status_bar *status_bar, struct mpd_status *status);
void draw_volume(struct status_bar *status_bar, struct mpd_status *status);
void draw_progress_bar(struct status_bar *status_bar, unsigned int time_elapsed, unsigned int song_length);
void draw_progress_label(struct status_bar *status_bar, unsigned int time_elapsed, unsigned int song_length);
void draw_song_label(struct status_bar *status_bar, struct mpd_song *song);
void draw_notification(struct status_bar *status_bar);

void set_notification(struct status_bar *status_bar, char *msg, int duration);

char *create_label_modes(char *buffer, struct mpd_status *status);
char *create_label_progress(char *buffer, unsigned int time_elapsed, unsigned int song_length);
char *create_label_song(char *buffer, const char *title, const char *artist);

#endif /* STATUSBAR_H */