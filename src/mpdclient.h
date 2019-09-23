/*******************************************************************************
 * mpdclient.h
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

#ifndef MPDCLIENT_H
#define MPDCLIENT_H

#include <mpd/client.h>

struct mpdclient {
    struct mpd_connection *connection;
    struct mpd_status *status;
    struct mpd_song *current_song;
    int queue_version;
    enum mpd_error last_error;
    enum mpd_state state;
};

struct mpdclient *mpdclient_init(const char *host, int port, int timeout);
void mpdclient_free(struct mpdclient *mpd);

#endif /* MPDCLIENT_H */
