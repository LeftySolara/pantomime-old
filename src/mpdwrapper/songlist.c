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
    if (node->song)
        mpd_song_free(node->song);

    node->prev = NULL;
    node->next = NULL;
    free(node);
}

/**
 * @brief Allocates memory for a list of songs.
 * 
 * @return An empty song list.
 */
struct songlist *songlist_init()
{
    struct songlist *list = malloc(sizeof(*list));

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/**
 * @brief Frees memory allocated by a song list.
 * 
 * @param list The list to free memory from.
 */
void songlist_free(struct songlist *list)
{
    songlist_clear(list);
    free(list);
}

/**
 * @brief Gets the song at the given index.
 * 
 * @param list The list to find a song from.
 * @param index The place in the list where the song is.
 * 
 * @return Pointer to the requested node, or NULL on error.
 */
struct songnode *songlist_at(struct songlist *list, unsigned int index)
{
    if (index >= list->size)
        return NULL;

    if (index == 0)
        return list->head;
    if (index == list->size-1)
        return list->tail;

    struct songnode *current = list->head;
    for (int i = 0; i < index; ++i)
        current = current->next;

    return current;
}

/**
 * @brief Adds a song to the end of the list.
 * 
 * @param list The list to append a song to.
 * @param song The song to add to the list. 
 *   If this is NULL, the function does nothing.
 */
void songlist_append(struct songlist *list, struct mpd_song *song)
{
    if (!song)
        return;

    struct songnode *new_node = songnode_init(song);

    if (!list->head)
        list->head = new_node;
    else {
        struct songnode *tmp = list->tail;
        tmp->next = new_node;
        new_node->prev = tmp;
    }

    list->tail = new_node;
    list->size++;
}

/**
 * @brief Removes the song at the given index from a list.
 * 
 * @param list The list to remove a song from.
 * @param index The position of the song to remove.
 */
void songlist_remove(struct songlist *list, unsigned int index)
{
    if (index >= list->size)
        return;

    if (index == 0) {
        struct songnode *tmp = list->head;

        list->head = list->head->next;
        list->head->prev = NULL;
        songnode_free(tmp);
    }
    else if (index == list->size-1) {
        struct songnode *tmp = list->tail;

        list->tail = list->tail->prev;
        list->tail->next = NULL;
        songnode_free(tmp);
    }
    else {
        struct songnode *tmp = songlist_at(list, index);

        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        songnode_free(tmp);
    }

    list->size--;
}

/**
 * @brief Removes all items from a list.
 * 
 * @param list The list to clear.
 */
void songlist_clear(struct songlist *list)
{
    struct songnode *current = list->head;
    struct songnode *next;

    while (current) {
        next = current->next;
        songnode_free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}