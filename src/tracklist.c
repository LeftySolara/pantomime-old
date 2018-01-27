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
    struct tracknode *node = malloc(sizeof(*node));

    node->next = NULL;
    node->prev = NULL;

    node->title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    node->artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    node->album = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);
    node->duration = mpd_song_get_duration(song);
    node->id = mpd_song_get_id(song);
    node->range_selected = false;
    return node;
}

struct tracklist *tracklist_init()
{
    struct tracklist *list = malloc(sizeof(*list));
    list->head = NULL;
    list->selected = NULL;

    return list;
}

void tracknode_free(struct tracknode *node)
{
    node->next = NULL;
    node->prev = NULL;
    free(node);
}

void tracklist_free(struct tracklist *list)
{
    tracklist_clear(list);
    free(list);
}

void tracklist_append(struct tracklist *list, struct mpd_song *song)
{
    struct tracknode *node = tracknode_init(song);

    if (!list->head) {
        /* If the new node is the only item in the list, select it */
        list->head = node;
        list->selected = list->head;
    }
    else {
        struct tracknode *current = list->head;
        while (current->next)
            current = current->next;
        current->next = node;
        node->prev = current;
    }
}

/* Remove the song at the current cursor position */
void tracklist_remove_selected(struct tracklist *list)
{
    if (!list->head || !list->selected)
        return;

    if (list->selected == list->head) {
        /* Only one node in list */
        if (!list->head->next) { 
            tracknode_free(list->head);
            list->head = NULL;
            list->selected = NULL;
        }
        else {
            struct tracknode *current = list->head;
            list->head = list->head->next;
            list->head->prev = NULL;
            tracknode_free(current);
        }
    }
    else {
        struct tracknode *current = list->selected;
        current->prev->next = current->next;
        current->next->prev = current->prev;

        if (current->next)
            list->selected = current->next;
        else
            list->selected = current->prev;

        tracknode_free(current);
    }
}

void tracklist_clear(struct tracklist *list)
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
}
