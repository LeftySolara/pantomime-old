/*******************************************************************************
 * songlist.h
 *******************************************************************************
 * Copyright (C) 2019  Jalen Adams
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
 * @brief A list of MPD songs.
 *
 * This file contains structures and functions for managing a list of songs.
 * The lists can be useful for representing the current play queue, or for
 * providing information to a GUI.
 */

#ifndef SONGLIST_H
#define SONGLIST_H

#include <mpd/client.h>
 
/**
 * @brief A song in the song list.
 */
struct songnode {
    struct mpd_song *song;  /**< The MPD song at this node. */
    struct songnode *next;  /**< The next song in the list. */
    struct songnode *prev;  /**< The previous song in the list. */
};

/**
 * @brief A list of MPD songs. Implemented as a doubly-linked list.
 */
struct songlist {
    struct songnode *head;  /**< The first item in the list. */
    struct songnode *tail;  /**< The last item in the list. */
    int size;               /**< The number of items in the list. */
};

struct songnode *songnode_init(struct mpd_song *song);
void songnode_free(struct songnode *node);

struct songlist *songlist_init();
void songlist_free(struct songlist *list);

struct songnode *songlist_at(struct songlist *list, unsigned int index);
void songlist_append(struct songlist *list, struct mpd_song *song);
void songlist_remove(struct songlist *list, unsigned int index);
void songlist_clear(struct songlist *list);

#endif /* SONGLIST_H */