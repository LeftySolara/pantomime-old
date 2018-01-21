/******************************************************************************
 * mpdclient.c : interface for interacting with MPD
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
#include <stdio.h>
#include "mpdclient.h"

struct mpdclient *mpdclient_init(char *host, int port, int timeout)
{
    struct mpdclient *mpd = malloc(sizeof(*mpd));

    /* Set some default values if necessary */
    mpd->host = (host == NULL) ? "localhost" : host;
    mpd->port = (port == 0) ? 6600 : port;
    mpd->timeout = (timeout == 0) ? 30000 : timeout;
    *(int *)&mpd->max_playlist_length = 16384;  /* Default maximum defined by MPD */

    mpd->connection = mpd_connection_new(mpd->host, mpd->port, mpd->timeout);
    mpd->status = mpd_run_status(mpd->connection);
    mpd->current_song = mpd_run_current_song(mpd->connection);
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->last_error= mpd_connection_get_error(mpd->connection);

    return mpd;
}

void mpdclient_free(struct mpdclient *mpd)
{
    if (mpd->connection)
        mpd_connection_free(mpd->connection);
    if (mpd->status)
        mpd_status_free(mpd->status);
    if (mpd->current_song)
        mpd_song_free(mpd->current_song);

    free(mpd);
}

void mpdclient_update(struct mpdclient *mpd)
{
    mpd->status = mpd_run_status(mpd->connection);
    mpd->current_song = mpd_run_current_song(mpd->connection);
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->last_error = mpd_connection_get_error(mpd->connection);
}
