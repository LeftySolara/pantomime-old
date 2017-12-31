/******************************************************************************
 * statusbar.c : interface for implementing a status bar
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

#include <stdlib.h>
#include <string.h>
#include "statusbar.h"

#define DURATION_FORMAT "[123:45/123:45]"

struct statusbar *statusbar_init(struct mpdclient *mpd)
{
    struct statusbar *statusbar = malloc(sizeof(*statusbar));

    statusbar->win = newwin(2, COLS, LINES - 2, 0);
    statusbar->queue_len = mpd_status_get_queue_length(mpd->status);

    return statusbar;
}

void statusbar_free(struct statusbar *statusbar)
{
    delwin(statusbar->win);
    statusbar->win = NULL;

    free(statusbar->duration_label);
    statusbar->duration_label = NULL;

    free(statusbar);
}

void statusbar_set_queue_length(struct statusbar *statusbar, struct mpdclient *mpd)
{
    statusbar->queue_len = mpd_status_get_queue_length(mpd->status);
}

void statusbar_set_duration_label(struct statusbar *statusbar, struct mpdclient *mpd)
{
    enum mpd_state state = mpd_status_get_state(mpd->status);
    if (state == MPD_STATE_UNKNOWN || state == MPD_STATE_STOP) {
        statusbar->duration_label = NULL;
        return;
    }

    unsigned int total_time = mpd_status_get_total_time(mpd->status);
    unsigned int elapsed_time = mpd_status_get_elapsed_time(mpd->status);

    if (total_time == 0)  /* no song playing */
        statusbar->duration_label = NULL;

    const size_t str_sz = strlen(DURATION_FORMAT) + 1;
    statusbar->duration_label = realloc(statusbar->duration_label,
                                        str_sz * sizeof(char));

    unsigned int total_minutes = total_time / 60;
    unsigned int total_seconds = total_time % 60;
    unsigned int elapsed_minutes = elapsed_time / 60;
    unsigned int elapsed_seconds = elapsed_time % 60;

    snprintf(statusbar->duration_label, str_sz, "[%d:%02d/%d:%02d]",
             elapsed_minutes, elapsed_seconds, total_minutes, total_seconds);
}
