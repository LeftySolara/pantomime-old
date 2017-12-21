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
#include "statusbar.h"

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
    free(statusbar);
}
