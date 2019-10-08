/*******************************************************************************
 * panel_queue.h
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

#ifndef PANEL_QUEUE_H
#define PANEL_QUEUE_H

#include "../mpdwrapper/mpdwrapper.h"

#include <ncurses.h>

/**
 * @brief A selectable item in the queue menu. Displays information about
 * a track in the current version of the play queue.
 */
struct queue_menu_item {
    struct queue_menu_item *prev;
    struct queue_menu_item *next;

    char *title;
    char *artist;
    char *album;

    unsigned int duration;  /* Length of the song in seconds. */
    unsigned int id;        /* ID given to the song by MPD if fetched from queue. */
};

struct queue_menu_list {
    struct queue_menu_item *head;
    struct queue_menu_item *tail;
    struct queue_menu_item *selected;

    int size;
};

struct queue_menu_item *queue_menu_item_init(struct mpd_song *song);
void queue_menu_item_free(struct queue_menu_item *item);

struct queue_menu_list *queue_menu_list_init(struct songlist *songlist);
void queue_menu_list_free(struct queue_menu_list *list);

void queue_menu_list_append(struct queue_menu_list *list, struct mpd_song *song);
void queue_menu_list_clear(struct queue_menu_list *list);

void draw_queue_menu_item(WINDOW *win, struct queue_menu_item *item, int field_width,
                          int begin_y, bool selected, bool playing);
void draw_queue(WINDOW *win, struct queue_menu_list *list, unsigned int playing_id);

#endif