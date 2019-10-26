/*******************************************************************************
 * playlist.c
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
 * @file playlist.h
 */

#include "playlist.h"

#include <stdlib.h>

/**
 * @brief Creates a new item to use in a playlist.
 * 
 * @param artist The song's artist
 * @param title  The song's title
 * @param album  The song's album
 * @param time   The song's length in seconds
 * @return       Pointer to a playlist item, or NULL on error.
 */
struct playlist_item *playlist_item_init(char *artist, char *title, char *album, int time)
{
    struct playlist_item *item = malloc(sizeof(*item));
    if (!item)
        return NULL;

    item->artist = artist;
    item->title = title;
    item->album = album;
    item->time = time;

    item->bold = 0;
    item->highlight = 0;

    item->prev = NULL;
    item->next = NULL;

    return item;
}

/**
 * @brief Frees memory allocated by a playlist item.
 */
void playlist_item_free(struct playlist_item *item)
{
    if (!item)
        return;

    free(item->artist);
    free(item->title);
    free(item->album);
    free(item);
}

/**
 * @brief Creates a new playlist that draws on the specified window.
 * 
 * @param win The window to draw the playlist on.
 * @return    Pointer to a new playlist, or NULL on error.
 */
struct playlist *playlist_init(WINDOW *win)
{
    struct playlist *playlist = malloc(sizeof(*playlist));
    if (!playlist)
        return NULL;

    playlist->win = win;

    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->selected = NULL;

    playlist->top_visible = NULL;
    playlist->bottom_visible = NULL;
    
    playlist->length = 0;
    playlist->idx_selected = -1;
    playlist->max_visible = getmaxx(win) - 1; /* Account for header row */

    return playlist;
}

/**
 * @brief Frees memory allocated by a playlist.
 */
void playlist_free(struct playlist *playlist)
{
    playlist_clear(playlist);
    free(playlist);
}

/**
 * @brief Adds an item to the end of a playlist.
 */
void playlist_append(struct playlist *playlist, struct playlist_item *item)
{
    if (!playlist || !item)
        return;
    
    if (!playlist->head) {
        playlist->head = item;
        playlist->selected = item;
        playlist->top_visible = item;
        playlist->bottom_visible = item;
        playlist->idx_selected = 0;
    }
    else {
        struct playlist_item *tmp = playlist->tail;
        tmp->next = item;
        item->prev = tmp;
    }

    playlist->tail = item;
    playlist->length++;
}

/**
 * @brief Populates the playlist with info from an MPD songlist.
 */
void playlist_populate(struct playlist *playlist, struct songlist *songlist)
{
    struct playlist_item *item;
    char *artist;
    char *title;
    char *album;
    int time;

    struct songnode *current = songlist->head;
    while (current) {
        artist = mpdwrapper_get_song_tag(current->song, MPD_TAG_ARTIST);
        title = mpdwrapper_get_song_tag(current->song, MPD_TAG_TITLE);
        album = mpdwrapper_get_song_tag(current->song, MPD_TAG_ALBUM);
        time = mpd_song_get_duration(current->song);

        item = playlist_item_init(artist, title, album, time);
        playlist_append(playlist, item);

        current = current->next;
    }

    playlist->idx_selected = 0;
    playlist->selected = playlist->head;
    playlist->top_visible = playlist->head;
    playlist_find_bottom(playlist);
}

/**
 * @brief Removes all items from a playlist.
 */
void playlist_clear(struct playlist *playlist)
{
    struct playlist_item *current = playlist->head;
    struct playlist_item *next;

    while (current) {
        next = current->next;
        playlist_item_free(current);
        current = next;
    }

    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->selected = NULL;
    playlist->top_visible = NULL;
    playlist->bottom_visible = NULL;
    playlist->length = 0;
    playlist->idx_selected = -1;
}

/**
 * @brief Calculates which playlist item is the bottommost visible.
 */
void playlist_find_bottom(struct playlist *playlist)
{
    if (!playlist)
        return;

    int rows = getmaxy(playlist->win);
    struct playlist_item *current = playlist->top_visible;

    for (int i = 0; i < rows - 2; ++i) {
        if (!current->next)
            break;
        current = current->next;
    }
    playlist->bottom_visible = current;
}