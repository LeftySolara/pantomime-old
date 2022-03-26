/*******************************************************************************
 * mpdwrapper.h
 *******************************************************************************
 * Copyright (C) 2019-2022  Julianne Adams
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

#ifndef MPDWRAPPER_INTERNAL_H
#define MPDWRAPPER_INTERNAL_H

#include <mpd/client.h>

#include "pantomime/mpdwrapper.h"

/**
 * @brief A node in a linked list of MPD songs.
 */
struct songnode {
    struct mpd_song *song; /**< The MPD song at this node. */
    struct songnode *next; /**< The next song in the list. */
    struct songnode *prev; /**< The previous song in the list. */
};

/**
 * @brief A doubly-linked list of MPD songs.
 */
struct songlist {
    struct songnode *head; /**< The first item in the list. */
    struct songnode *tail; /**< The last item in the list. */
    int size;              /**< The number of items in the list. */
};

/**
 * @brief Holds information about the current MPD server connection.
 *
 * This struct contains information about the current MPD server connection.
 * With this struct, we are able to keep track of the connection state
 * without having to make continual (often unnecessary) server requests.
 */
struct mpdwrapper {
    struct mpd_connection *connection; /**< The MPD server connection. */
    struct mpd_status *status;         /**< Holds info about MPD's status. */
    struct mpd_song *current_song;     /**< The currently playing song. */
    struct songlist *queue;    /**< A songlist struct representing the current play queue. */
    enum mpd_error last_error; /**< The most recent error encountered by MPD. */
    enum mpd_state state;      /**< Current player state (playing, paused, or stopped). */
    int queue_version;  /**< The queue version number. Useful for checking if queue has changed. */
    bool queue_changed; /**< Whether the queue has changed since the last refresh. */
};

struct songnode *songnode_new(struct mpd_song *song);
void songnode_initialize(struct songnode *node, struct mpd_song *song);
void songnode_free(struct songnode *node);

void songlist_initialize(struct songlist *songlist);
struct songnode *songlist_node_at(struct songlist *songlist, unsigned int index);
int songlist_get_size(struct songlist *songlist);

void mpdwrapper_initialize(struct mpdwrapper *mpd, const char *host, int port, int timeout);
void mpdwrapper_fetch_queue(struct mpdwrapper *mpd);

#endif /* MPDWRAPPER_INTERNAL_H */
