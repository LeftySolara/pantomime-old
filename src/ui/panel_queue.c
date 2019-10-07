/*******************************************************************************
 * panel_queue.c
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
 * @file panel_queue.h
 * 
 */

#include "panel_queue.h"

#include <stdlib.h>

struct queue_menu_item *queue_menu_item_init(struct mpd_song *song)
{
    if (!song)
        return NULL;
    
    struct queue_menu_item *item = malloc(sizeof(*item));
    item->prev = NULL;
    item->next = NULL;

    item->title = mpdwrapper_get_song_tag(song, MPD_TAG_TITLE);
    item->artist = mpdwrapper_get_song_tag(song, MPD_TAG_ARTIST);
    item->album = mpdwrapper_get_song_tag(song, MPD_TAG_ALBUM);

    item->duration = mpd_song_get_duration(song);
    item->id = mpd_song_get_id(song);

    return item;
}

void queue_menu_item_free(struct queue_menu_item *item)
{
    if (!item)
        return;
    
    free(item->title);
    free(item->artist);
    free(item->album);

    free(item);
}

struct queue_menu_list *queue_menu_list_init(struct songlist *songlist)
{
    struct queue_menu_list *menu_list = malloc(sizeof(*menu_list));

    struct songnode *current = songlist->head;
    while (current) {
        queue_menu_list_append(menu_list, current->song);
        current = current->next;
    }

    menu_list->selected = NULL;
    return menu_list;
}

void queue_menu_list_free(struct queue_menu_list *list)
{
    queue_menu_list_clear(list);
    free(list);
}

void queue_menu_list_append(struct queue_menu_list *list, struct mpd_song *song)
{
    if (!song)
        return;
    
    struct queue_menu_item *new_item = queue_menu_item_init(song);

    if (!list->head)
        list->head = new_item;
    else {
        struct queue_menu_item *tmp = list->tail;
        tmp->next = new_item;
        new_item->prev = tmp;
    }

    list->tail = new_item;
    list->size++;
}

void queue_menu_list_clear(struct queue_menu_list *list)
{
    struct queue_menu_item *current = list->head;
    struct queue_menu_item *next;

    while (current) {
        next = current->next;
        queue_menu_item_free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->selected = NULL;
    list->size = 0;
}

void draw_queue(WINDOW *win, struct mpdwrapper *mpd)
{
    werase(win);

    int y = 0;

    struct songnode *node = mpd->queue->head;
    while (node) {
        mvwaddstr(win, y++, 0, mpd_song_get_tag(node->song, MPD_TAG_TITLE, 0));
        node = node->next;
    }

    wnoutrefresh(win);
}