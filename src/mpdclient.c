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

struct mpd *mpd_connect(const char *host, int port, int timeout)
{
    /* Set some default values */
    if (host == NULL)
        host = "localhost";
    if (port == 0)
        port = 6600;
    if (timeout == 0)
        timeout = 30000;

    struct mpd *mpd = malloc(sizeof(mpd));
    mpd->connection = mpd_connection_new(host, port, timeout);
    mpd->last_error= mpd_connection_get_error(mpd->connection);

    return mpd;
}
