/*******************************************************************************
 * mpdwrapper.c
 *******************************************************************************
 * Copyright (C) 2019  Jalen Adams
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "mpdwrapper.h"

#include <stdlib.h>
#include <stdio.h>


struct mpdwrapper *mpdwrapper_init(const char *host, int port, int timeout)
{
    struct mpdwrapper *mpd = malloc(sizeof(*mpd));

    mpd->connection = mpd_connection_new(host, port, timeout);
    mpd->status = mpd_run_status(mpd->connection);
    mpd->current_song = mpd_run_current_song(mpd->connection);
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    return mpd;
}

void mpdwrapper_free(struct mpdwrapper *mpd)
{
    mpd_status_free(mpd->status);
    mpd_song_free(mpd->current_song);
    mpd_connection_free(mpd->connection);

    free(mpd);
}

void mpdwrapper_update(struct mpdwrapper *mpd)
{
    mpd->status = mpd_run_status(mpd->connection);
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->current_song = mpd_run_current_song(mpd->connection);
    mpd->last_error = mpd_connection_get_error(mpd->connection);
}