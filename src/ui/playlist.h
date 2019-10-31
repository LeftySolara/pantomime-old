/*******************************************************************************
 * playlist.h
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
 * @brief UI elements for displaying a playlist.
 * 
 * The structures and functions in this file are used for drawing a playlist on-screen.
 * Text for UI elements is populated from a [songlist](@ref songlist.h) structure, which contains the underlying
 * MPD playlist.
 * 
 * This is for displaying any type of playlist (stored playlists, the play queue, etc.).
 * MPD uses the terms "playlist" and "queue" interchangeably when referring to the current
 * play queue. Here we use "playlist" to mean any type of playlist, and only refer
 * to the play queue as "the queue".
 */

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "../mpdwrapper/mpdwrapper.h"

#include <ncurses.h>

/**
 * @brief A row in the playlist display, representing one song.
 */
struct playlist_item {
    char *artist;   /**< The song's artist. */
    char *title;    /**< The song's title. */
    char *album;    /**< The song's album. */
    int time;       /**< Length of the song in seconds. */
    unsigned id;    /**< The MPD ID of the song. */

    int bold;       /**< Whether to print this item's text in bold. */
    int highlight;  /**< Whether to highlight this item after printing. */

    struct playlist_item *prev; /**< The next song in the playlist. */
    struct playlist_item *next; /**< The previous song in the playlist. */
};

/**
 * @brief A navigable playlist.
 * 
 * The playlist UI is drawn as a simple list with song information.
 * Functions that navigate through or manipulate the list are
 * triggered by the user with the [command interface](@ref command.h).
 */
struct playlist {
    WINDOW *win;  /**< The ncurses window to draw the playlist on. */

    struct playlist_item *head;     /**< The first item in the list. */
    struct playlist_item *tail;     /**< The last item in the list. */
    struct playlist_item *selected; /**< The currently selected item. */

    struct playlist_item *top_visible;      /**< First visible item in the window. */
    struct playlist_item *bottom_visible;   /**< Last visible item in the window. */

    int length;         /**< The number of items in the playlist. MPD's default maximum is 4096. */
    int idx_selected;   /**< The index of the currently selected item. */
    int max_visible;    /**< The maximum number of items that can be displayed with the current window size. */
};

struct playlist_item *playlist_item_init(char *artist, char *title, char *album, int time, unsigned id);
void playlist_item_free(struct playlist_item *item);

struct playlist *playlist_init(WINDOW *win);
void playlist_free(struct playlist *playlist);

void playlist_append(struct playlist *playlist, struct playlist_item *item);
bool playlist_remove_selected(struct playlist *playlist);

void playlist_populate(struct playlist *playlist, struct songlist *songlist);
void playlist_clear(struct playlist *playlist);

void playlist_set_selected(struct playlist *playlist, int idx);
void playlist_select_prev(struct playlist *playlist);
void playlist_select_next(struct playlist *playlist);
void playlist_select_top_visible(struct playlist *playlist);
void playlist_select_bottom_visible(struct playlist *playlist);
void playlist_select_middle_visible(struct playlist *playlist);

void playlist_scroll_page_up(struct playlist *playlist);
void playlist_scroll_page_down(struct playlist *playlist);

void playlist_find_bottom(struct playlist *playlist);
int playlist_find_cursor_pos(struct playlist *playlist);

void playlist_item_draw(struct playlist_item *item, WINDOW *win, unsigned y, unsigned field_width);
void playlist_deaw_header(struct playlist *playlist, unsigned field_width);
void playlist_draw(struct playlist *playlist, unsigned playing_id);

#endif /* PLAYLIST_H */