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
#include <ncurses.h>
#include "statusbar.h"

#define QUEUE_LEN_STRING "songs in queue"
#define MODE_OFF_CHAR '-'

struct statusbar *statusbar_init()
{
    struct statusbar *statusbar = malloc(sizeof(*statusbar));
    statusbar->win = newwin(2, COLS, LINES - 2, 0);

    return statusbar;
}

void statusbar_free(struct statusbar *statusbar)
{
    delwin(statusbar->win);
    statusbar->win = NULL;

    free(statusbar);
}

void statusbar_draw_progress(struct statusbar *statusbar)
{
    enum mpd_state state = mpd_status_get_state(mpdclient->status);
    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN)
        return;

    double song_length = mpd_song_get_duration(mpdclient->current_song);
    double time_elapsed = mpd_status_get_elapsed_time(mpdclient->status);
    double width = getmaxx(statusbar->win);

    double secs_per_tick = song_length / width;     /* number of seconds before the bar progresses */
    double tick_size = (width / song_length) + 1;   /* number of characters to print per tick */
    double ticks_elapsed = time_elapsed / tick_size;

    whline(statusbar->win, '-', (tick_size * ticks_elapsed) / secs_per_tick);
    mvwaddch(statusbar->win, 0, (tick_size * ticks_elapsed) / secs_per_tick, '>');
}

void statusbar_draw(struct statusbar *statusbar, struct ui *ui)
{
    wclear(statusbar->win);
    statusbar_draw_progress(statusbar);
    mvwaddstr(statusbar->win, 1, 0, ui->label_queue);
    mvwaddstr(statusbar->win, 1, COLS - strlen(ui->label_modes),
              ui->label_modes);

    wnoutrefresh(statusbar->win);
}
