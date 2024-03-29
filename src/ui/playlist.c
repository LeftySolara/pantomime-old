/*******************************************************************************
 * playlist.c
 *******************************************************************************
 * Copyright (C) 2019-2022  Julianne Adams
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
 * @param artist The song's artist.
 * @param title  The song's title.
 * @param album  The song's album.
 * @param time   The song's length in seconds.
 * @param id     The song's MPD id.
 * @return       Pointer to a playlist item, or NULL on error.
 */
struct playlist_item *playlist_item_init(char *artist, char *title, char *album, int time,
                                         unsigned id)
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
 * @brief Creates a new (empty) playlist UI that draws on the specified window.
 *
 * @param win The ncurses window to draw the playlist on.
 * @return    Pointer to a new playlist struct, or NULL on error.
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
    playlist->idx_last_top = -1;
    playlist->idx_selected = -1;
    playlist->max_visible = getmaxy(win) - 1; /* -1 to account for header row */

    return playlist;
}

/**
 * @brief Frees memory allocated by a playlist UI.
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
 * @brief Removes the currently selected song from the playlist.
 *
 * This function removes the selected song's entry from the playlist display.
 * This is meant to be used alongside mpdwrapper_delete_from_queue(), which
 * tells the MPD connection to remove the song on the back-end.
 *
 * @return True on success, or false on error.
 */
bool playlist_remove_selected(struct playlist *playlist)
{
    if (!playlist->head)
        return false;

    if (playlist->selected == playlist->head) {
        if (!playlist->head->next) /* Only one song in the list */
            playlist_clear(playlist);
        else {
            struct playlist_item *current = playlist->head;
            playlist_select_next(playlist);
            playlist->top_visible = playlist->selected;
            playlist_find_bottom(playlist);
            playlist->head->prev = NULL;

            playlist_item_free(current);
            playlist->head = playlist->selected;
        }
        playlist->length--;
        playlist->idx_selected = 0;
    }
    else {
        struct playlist_item *current = playlist->selected;

        if (current->next)
            playlist_select_next(playlist);
        else
            playlist_select_prev(playlist);

        if (current == playlist->top_visible)
            playlist->top_visible = playlist->selected;
        if (current == playlist->tail) {
            playlist->tail = playlist->tail->prev;
            playlist->idx_selected++; /* Fixes weird off-by-one error */
        }

        if (current->prev)
            current->prev->next = current->next;
        if (current->next)
            current->next->prev = current->prev;

        playlist_item_free(current);
        playlist->length--;
        playlist->idx_selected--;
        playlist_find_bottom(playlist);
    }

    return true;
}

/**
 * @brief Populates the playlist UI with info from a [songlist](@ref songlist.h).
 */
void playlist_populate(struct playlist *playlist, struct songlist *songlist)
{
    struct playlist_item *item;
    struct mpd_song *song;
    char *artist;
    char *title;
    char *album;
    int time;
    unsigned id;

    int length = songlist_get_size(songlist);
    for (int i = 0; i < length; ++i) {
        song = songlist_at(songlist, i);

        artist = mpdwrapper_get_song_tag(song, MPD_TAG_ARTIST);
        title = mpdwrapper_get_song_tag(song, MPD_TAG_TITLE);
        album = mpdwrapper_get_song_tag(song, MPD_TAG_ALBUM);
        time = mpd_song_get_duration(song);
        id = mpd_song_get_id(song);

        item = playlist_item_init(artist, title, album, time, id);
        playlist_append(playlist, item);
    }

    playlist_restore_last_top(playlist);
    playlist_set_selected(playlist, playlist->idx_last_selected);
    if (playlist->selected)
        playlist->selected->highlight = 1;
    playlist_find_bottom(playlist);
}

/**
 * @brief Removes all items from the playlist UI.
 */
void playlist_clear(struct playlist *playlist)
{
    struct playlist_item *current = playlist->head;
    struct playlist_item *next;

    int idx = 0;
    while (current) {
        if (current == playlist->top_visible)
            playlist->idx_last_top = idx;
        next = current->next;
        playlist_item_free(current);
        current = next;
        ++idx;
    }

    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->selected = NULL;
    playlist->top_visible = NULL;
    playlist->bottom_visible = NULL;
    playlist->length = 0;
    playlist->idx_last_selected = playlist->idx_selected;
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
 * @brief Sets the first visible item in the window as the selected item.
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
 * @brief Sets the last visible item in the window as the selected item.
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
 * @brief Sets the item in the middle of the window as the selected item.
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

    /* For restoring the cursor position, if necessary. */
    int y_pos = playlist_find_cursor_pos(playlist);

    playlist_select_top_visible(playlist);
    if (!playlist->selected->prev)
        return;

    for (int i = 0; i <= playlist->max_visible - 1; ++i)
        playlist_select_prev(playlist);

    /* Restore cursor position */
    for (int i = 0; i < y_pos; ++i)
        playlist_select_next(playlist);
}

/**
 * @brief Scrolls down one page in the playlist.
 */
void playlist_scroll_page_down(struct playlist *playlist)
{
    if (!playlist || playlist->length == 0)
        return;

    /* For restoring cursor position, if necessary. */
    int y_pos = playlist_find_cursor_pos(playlist);

    playlist_select_bottom_visible(playlist);
    if (!playlist->selected->next)
        return;

    playlist_select_next(playlist);
    playlist->top_visible = playlist->selected;
    playlist_find_bottom(playlist);

    /* Restore cursor position */
    playlist_select_top_visible(playlist);
    for (int i = 0; i < y_pos; ++i)
        playlist_select_next(playlist);
}

/**
 * @brief Calculates which playlist item is the bottommost visible.
 */
void playlist_find_bottom(struct playlist *playlist)
{
    if (!playlist || !playlist->head)
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
 * @brief Finds the y-position of the cursor in the window.
 */
int playlist_find_cursor_pos(struct playlist *playlist)
{
    if (!playlist->top_visible)
        return -1;

    struct playlist_item *current = playlist->top_visible;
    int i = 0;

    while (current != playlist->selected) {
        ++i;
        current = current->next;
    }

    return i;
}

void playlist_restore_last_top(struct playlist *playlist)
{
    if (playlist->idx_last_top < 0)
        playlist->top_visible = playlist->head;
    else
        playlist->top_visible = playlist_at(playlist, playlist->idx_last_top);
}

/**
 * @brief Finds the playlist item at the given index.
 *
 * @param playlist The playlist to search.
 * @param idx The index to check.
 * @return struct playlist_item*
 */
struct playlist_item *playlist_at(struct playlist *playlist, int index)
{
    if (index >= playlist->length)
        return NULL;

    if (index == 0)
        return playlist->head;
    if (index == playlist->length - 1)
        return playlist->tail;

    struct playlist_item *current = playlist->head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }

    return current;
}

/**
 * @brief Draws a playlist item on the specified window.
 *
 * @param item          The item to draw.
 * @param win           The window to draw on.
 * @param y             The y-position for drawing.
 * @param field_width   The number of characters in each column.
 */
void playlist_item_draw(struct playlist_item *item, WINDOW *win, unsigned y, unsigned field_width)
{
    int maxx = getmaxx(win);

    if (item->bold)
        wattr_on(win, A_BOLD, 0);
    if (item->highlight)
        wattr_on(win, A_STANDOUT, 0);

    mvwprintw(win, y, 0, "%.*s\n", field_width - 2, item->artist);
    mvwprintw(win, y, field_width + 1, "%.*s\n", field_width - 2, item->title);
    mvwprintw(win, y, (field_width * 2) + 1, "%.*s\n", field_width - 2, item->album);
    mvwprintw(win, y, maxx - 8, "%d:%02d\n", item->time / 60,
              item->time % 60); /* "Length" column has a fixed width */

    if (item->highlight)
        mvwchgat(win, y, 0, -1, A_STANDOUT, 0, NULL);
    wattr_off(win, A_BOLD, 0);
    wattr_off(win, A_STANDOUT, 0);
}

/**
 * @brief Draws the playlist header.
 *
 * @param playlist      The playlist whose header to draw.
 * @param field_width   The number of characters in each column.
 */
void playlist_deaw_header(struct playlist *playlist, unsigned field_width)
{
    int maxx = getmaxx(playlist->win);

    wattr_on(playlist->win, A_BOLD, NULL);
    mvwprintw(playlist->win, 0, 0, "Artist\n");
    mvwprintw(playlist->win, 0, field_width, " Title\n");
    mvwprintw(playlist->win, 0, (field_width * 2), " Album\n");
    mvwprintw(playlist->win, 0, maxx - 9, " Length\n");
    wattr_off(playlist->win, A_BOLD, NULL);
}

/**
 * @brief Draws a playlist on the screen.
 *
 * @param playlist      The playlist to draw.
 * @param playing_id    The MPD id of the currently playing song.
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

    /* Trying to draw the whole list at once and scrolling thorugh it
     * doesn't work because drawing past the bounds of an ncurses window
     * does nothing (this can kind of work with an ncurses pad, but that doesn't
     * allow for list selection, navigation, and manipulation in the way we want).
     * Because of this, it's more efficient to just worry about the items
     * that are currently visible and redraw manually when there's a change.
     *
     * Since we know which item is displayed at the top and have the window dimensions,
     * we can figure out which item will be the last one visible and only draw
     * the ones in that range.
     */
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
