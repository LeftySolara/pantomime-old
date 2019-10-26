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
 * @param id     The song's MPD id
 * @return       Pointer to a playlist item, or NULL on error.
 */
struct playlist_item *playlist_item_init(char *artist, char *title, char *album, int time, unsigned id)
{
    struct playlist_item *item = malloc(sizeof(*item));
    if (!item)
        return NULL;

    item->artist = artist;
    item->title = title;
    item->album = album;
    item->time = time;
    item->id = id;

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
    playlist->max_visible = getmaxy(win) - 1; /* Account for header row */

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
    unsigned id;

    struct songnode *current = songlist->head;
    while (current) {
        artist = mpdwrapper_get_song_tag(current->song, MPD_TAG_ARTIST);
        title = mpdwrapper_get_song_tag(current->song, MPD_TAG_TITLE);
        album = mpdwrapper_get_song_tag(current->song, MPD_TAG_ALBUM);
        time = mpd_song_get_duration(current->song);
        id = mpd_song_get_id(current->song);

        item = playlist_item_init(artist, title, album, time, id);
        playlist_append(playlist, item);

        current = current->next;
    }

    playlist->idx_selected = 0;
    playlist->selected = playlist->head;
    playlist->top_visible = playlist->head;
    playlist->selected->highlight = 1;
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
 * @brief Set the item at the specified index as the currently selected item.
 */
void playlist_set_selected(struct playlist *playlist, int idx)
{
    if (idx < 0 || idx >= playlist->length)
        return;
    
    struct playlist_item *current = playlist->head;
    for (int i = 0; i < idx; ++i)
        current = current->next;

    playlist->selected = current;
    playlist->idx_selected = idx;
    current->highlight = 1;

    current = current->next;
    while (current) {
        current->highlight = 0;
        current = current->next;
    }
}

/**
 * @brief Selects the previous item in the playlist.
 */
void playlist_select_prev(struct playlist *playlist)
{
    if (!playlist || playlist->selected == playlist->head)
        return;

    struct playlist_item *current = playlist->selected;
    current->highlight = 0;
    if (current == playlist->top_visible) {
        playlist->top_visible = current->prev;
        playlist_find_bottom(playlist);
    }

    current = current->prev;
    current->highlight = 1;

    playlist->selected = playlist->selected->prev;
    playlist->idx_selected--;
}

/**
 * @brief Selects the next item in the playlist.
 */
void playlist_select_next(struct playlist *playlist)
{
    if (!playlist || playlist->selected == playlist->tail)
        return;

    struct playlist_item *current = playlist->selected;
    current->highlight = 0;
    if (current == playlist->bottom_visible && current->next) {
        playlist->top_visible = playlist->top_visible->next;
        playlist_find_bottom(playlist);
    }

    current = current->next;
    current->highlight = 1;

    playlist->selected = playlist->selected->next;
    playlist->idx_selected++;
}

/**
 * @brief Selects the first visible item in the playlist.
 */
void playlist_select_top_visible(struct playlist *playlist)
{
    if (!playlist || playlist->selected == playlist->top_visible)
        return;

    struct playlist_item *current = playlist->selected;
    current->highlight = 0;

    while (current != playlist->top_visible) {
        current = current->prev;
        playlist->idx_selected--;
    }

    current->highlight = 1;
    playlist->selected = current;
}

/**
 * @brief Selects the last visible item in the playlist.
 */
void playlist_select_bottom_visible(struct playlist *playlist)
{
    if (!playlist || playlist->selected == playlist->bottom_visible)
        return;
    
    struct playlist_item *current = playlist->selected;
    current->highlight = 0;

    while (current != playlist->bottom_visible) {
        current = current->next;
        playlist->idx_selected++;
    }

    current->highlight = 1;
    playlist->selected = current;
}

/**
 * @brief Selects the item in the middle of the visible playlist items.
 */
void playlist_select_middle_visible(struct playlist *playlist)
{
    if (!playlist || playlist->length == 0)
        return;

    int midpoint = playlist->max_visible / 2;

    playlist_select_top_visible(playlist);
    for (int i = 0; i < midpoint; ++i)
        playlist_select_next(playlist);
}

/**
 * @brief Scrolls up one page in the playlist.
 */
void playlist_scroll_page_up(struct playlist *playlist)
{
    if (!playlist || playlist->length == 0)
        return;

    for (int i = 0; i <= playlist->max_visible; ++i)
        playlist_select_prev(playlist);
}

/**
 * @brief Scrolls down one page in the playlist.
 */
void playlist_scroll_page_down(struct playlist *playlist)
{
    if (!playlist || playlist->length == 0)
        return;

    playlist_select_bottom_visible(playlist);
    if (!playlist->selected->next)
        return;

    playlist_select_next(playlist);
    playlist->top_visible = playlist->selected;
    playlist_find_bottom(playlist);
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

/**
 * @brief Draws a playlist item on the specified window.
 * 
 * @param item          The item to draw
 * @param win           The window to draw on
 * @param y             The y-position for drawing
 * @param field_width   The number of characters in each column
 */
void playlist_item_draw(struct playlist_item *item, WINDOW *win, unsigned y, unsigned field_width)
{
    int maxx = getmaxx(win);

    if (item->bold)
        wattr_on(win, A_BOLD, 0);
    if (item->highlight)
        wattr_on(win, A_STANDOUT, 0);

    mvwprintw(win, y, 0, "%s\n", item->artist);
    mvwprintw(win, y, field_width + 1, "%s\n", item->title);
    mvwprintw(win, y, (field_width * 2) + 1, "%s\n", item->album);
    mvwprintw(win, y, maxx - 8, "%d:%02d\n", item->time / 60, item->time % 60);

    if (item->highlight)
        mvwchgat(win, y, 0, -1, A_STANDOUT, 0, NULL);
    wattr_off(win, A_BOLD, 0);
    wattr_off(win, A_STANDOUT, 0);
}

/**
 * @brief Draws the playlist header.
 * 
 * @param playlist      The playlist whose header to draw
 * @param field_width   The number of characters in each column
 */
void playlist_deaw_header(struct playlist *playlist, unsigned field_width)
{
    int maxx = getmaxx(playlist->win);

    wattr_on(playlist->win, A_BOLD, NULL);
    mvwprintw(playlist->win, 0, 0, "Artist\n");
    mvwprintw(playlist->win, 0, field_width + 1, " Title\n");
    mvwprintw(playlist->win, 0, (field_width * 2) + 1, " Album\n");
    mvwprintw(playlist->win, 0, maxx - 9, " Length\n");
    wattr_off(playlist->win, A_BOLD, NULL);
}

/**
 * @brief Draws a playlist on the screen.
 * 
 * @param playlist      The playlist to draw
 * @param playing_id    The MPD id of the currently playing song
 */
void playlist_draw(struct playlist *playlist, unsigned playing_id)
{
    werase(playlist->win);

    int maxx = getmaxx(playlist->win);
    int field_width = (maxx - 8) / 3;
    playlist_deaw_header(playlist, field_width);

    if (playlist->length <= 0)
        return;
    if (!playlist->selected)
        playlist_select_top_visible(playlist);

    struct playlist_item *current = playlist->top_visible;
    playlist_find_bottom(playlist);

    int y = 1;
    while (current != playlist->bottom_visible->next) {
        if (current->id == playing_id)
            current->bold = 1;
        else
            current->bold = 0;
        
        playlist_item_draw(current, playlist->win, y++, field_width);
        current = current->next;
    }

    wnoutrefresh(playlist->win);
}