/*******************************************************************************
 * mpdwrapper.c
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

#include <stdlib.h>

#include "mpdwrapper.h"

/**
 * @brief Connect to MPD and create an mpdwrapper object.
 *
 * @param host The hostname, IP address, or UNIX socket of the MPD server.
 * @param port The TCP port to connect to, if using an IP address.
 * @param timeout The timeout in milliseconds.

 * @return A pointer to the newly-allocated mpdwrapper object.
 */
struct mpdwrapper *mpdwrapper_init(const char *host, int port, int timeout)
{
    struct mpdwrapper *mpd = malloc(sizeof(*mpd));
    if (!mpd) {
        return NULL;
    }

    mpd->connection = mpd_connection_new(host, port, timeout);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    return mpd;
}

/**
 * @brief Disconnect from MPD and destroy an mpdwrapper object.
 *
 * @param mpd The connection to destroy.
 */
void mpdwrapper_free(struct mpdwrapper *mpd)
{
    mpd_connection_free(mpd->connection);
    free(mpd);
}

/**
 * @brief Get the hostname of the current MPD connection.
 *
 * @param mpd The MPD connection to check.

 * @return A string containing the name of the connection host.
 */
const char *mpdwrapper_get_host(struct mpdwrapper *mpd)
{
    const struct mpd_settings *settings = mpd_connection_get_settings(mpd->connection);
    return mpd_settings_get_host(settings);
}

/**
 * @brief Get the most recent error message produced by MPD.
 *
 * @param mpd The MPD connection to check.
 * @return The most recent error message from an MPD connection.
 */
const char *mpdwrapper_get_last_error_message(struct mpdwrapper *mpd)
{
    return mpd_connection_get_error_message(mpd->connection);
}
