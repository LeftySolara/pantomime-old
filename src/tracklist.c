/******************************************************************************
 * tracklist.c : interface for displaying track lists
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
#include "mpdclient.h"
#include "tracklist.h"

struct tracknode *tracknode_init(struct mpd_song *song)
{
    struct tracknode *node = malloc(sizeof(struct tracknode *));

    node->next = NULL;
    node->prev = NULL;
    node->song = song;
    node->selected = false;
    node->playing = mpd_status_get_song_id(mpdclient->status)
        == mpd_song_get_id(node->song);

    return node;
}

struct tracklist *tracklist_init()
{
    struct tracklist *list = malloc(sizeof(struct tracklist *));
    list->head = NULL;
    list->selected = NULL;
}

void tracknode_free(struct tracknode *node)
{
    mpd_song_free(node->song);
    node->next = NULL;
    node->prev = NULL;
    free(node);
}

void tracklist_free(struct tracklist *list)
{
    struct tracknode *current = list->head;
    while (current) {
        list->head = current->next;
        tracknode_free(current);
        current = list->head;
    }

    if (list->selected)
        tracknode_free(list->selected);
    list->selected = NULL;
    free(list);
}
