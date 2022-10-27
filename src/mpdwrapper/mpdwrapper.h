/*******************************************************************************
 * mpdwrapper.h
 *******************************************************************************
 * Copyright (C) 2017-2022  Julianne Adams
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
 * @file mpdwrapper.h
 */

#ifndef MPDWRAPPER_H
#define MPDWRAPPER_H

#include <mpd/client.h>
#include <mpd/error.h>

/**
 * @brief A wrapper for an MPD server connection.
 *
 * This struct contains information about an MPD server connection.
 * With this struct, we are able to keep track of connection state
 * and other information without having to constantly make unnecessary
 * server requests.
 */
struct mpdwrapper {
    struct mpd_connection *connection; /**< The MPD server connection. */
    enum mpd_error last_error;         /**< The most recent error encountered by MPD. */
};

struct mpdwrapper *mpdwrapper_init(const char *host, int port, int timeout);
void mpdwrapper_free(struct mpdwrapper *mpd);

const char *mpdwrapper_get_last_error_message(struct mpdwrapper *mpd);

#endif /* MPDWRAPPER_H */
