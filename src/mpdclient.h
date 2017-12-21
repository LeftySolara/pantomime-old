/******************************************************************************
 * mpdclient.h : interface for interacting with MPD
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

#ifndef MPDCLIENT_H
#define MPDCLIENT_H

#include <mpd/client.h>

struct mpdclient {
    struct mpd_connection *connection;
    struct mpd_status *status;
    enum mpd_error last_error;
    int current_song_id;

    char *host;
    int port;
    int timeout;
};

extern struct mpdclient *mpdclient;

struct mpdclient *mpdclient_connect(char *host, int port, int timeout);
void mpdclient_free(struct mpdclient *mpd);

#endif
