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

    if (!list->head) {
        list->head = new_item;
        list->selected = new_item;
    }
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

void draw_queue_menu_item(WINDOW *win, struct queue_menu_item *item, int field_width,
                          int row, bool selected, bool playing)
{
    int maxx = getmaxx(win);

    if (playing)    /* Bold the currently playing song */
        wattr_on(win, A_BOLD, 0);
    if (selected)   /* Highlight selected items */
        wattr_on(win, A_STANDOUT, 0);
    
    mvwprintw(win, row, 0, "%s\n", item->artist); 
    mvwprintw(win, row, field_width + 1, " %s\n", item->title);
    mvwprintw(win, row, (field_width * 2) + 1, " %s\n", item->album);
    mvwprintw(win, row, maxx - 8, "%d:%02d\n", item->duration / 60, item->duration % 60);

    if (selected)
        mvwchgat(win, row, 0, -1, A_STANDOUT, 0, NULL);
    wattr_off(win, A_BOLD, 0);
    wattr_off(win, A_STANDOUT, 0);
}

void draw_queue(WINDOW *win, struct queue_menu_list *list, unsigned int playing_id)
{
    int y, x;
    getmaxyx(win, y, x);

    /* The width of each column. There are four columns total but only three
     * are of variable size; the "Length" column is always 8 characters wide.
     */
    int field_width = (x - 8) / 3;

    werase(win);

    /* Draw column titles */
    wattr_on(win, A_BOLD, NULL);
    mvwprintw(win, 0, 0, "Artist\n");
    mvwprintw(win, 0, field_width + 1, " Title\n");
    mvwprintw(win, 0, (field_width * 2) + 1, " Album\n");
    mvwprintw(win, 0, x - 9, " Length\n");
    wattr_off(win, A_BOLD, NULL);

    if (list->size <= 0)
        return;
    if (!list->selected)
        list->selected = list->head;

    struct queue_menu_item *item = list->head;
    bool selected;
    bool playing;

    /* Only draw y items to prevent wasting resources
     * drawing past the end of the screen.
     */
    for (int i = 1; i <= y; ++i) {
        playing = item->id == playing_id;
        selected = item->id == list->selected->id;

        draw_queue_menu_item(win, item, field_width, i, selected, playing);
        if (!item->next)
            break;
        else
            item = item->next;
    }
    wnoutrefresh(win);
}