/*******************************************************************************
 * songlist.c
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
 * @file songlist.h
 */

#include "songlist.h"

#include <stdlib.h>

/**
 * @brief Creates a new song for a list.
 * 
 * @param song The song to add to the list.
 * @return A song node.
 */
struct songnode *songnode_init(struct mpd_song *song)
{
    struct songnode *node = malloc(sizeof(*node));

    node->song = song;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

/**
 * @brief Frees memory allocated by a list node.
 * 
 * @param node The node to free memory from.
 */
void songnode_free(struct songnode *node)
{
    mpd_song_free(node->song);
    node->prev = NULL;
    node->next = NULL;
    free(node);
}