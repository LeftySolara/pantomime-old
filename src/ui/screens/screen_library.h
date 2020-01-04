/*******************************************************************************
 * screen_library.h
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
 * @file screen_library.h
 */

#ifndef SCREEN_LIBRARY_H
#define SCREEN_LIBRARY_H

#include "../views/list_view.h"
#include "pantomime/mpdwrapper.h"

struct screen_library {
    struct list_view *artist_list_view;
    struct list_view *album_list_view;
    struct list_view *song_list_view;
    struct list_view *visible_view;
};

struct screen_library *screen_library_new(int height, int width);
void screen_library_initialize(struct screen_library *screen, int height, int width);
void screen_library_free(struct screen_library *screen);

void screen_library_populate_artists(struct screen_library *screen, struct mpdwrapper *mpd);
void screen_library_populate_albums(struct screen_library *screen, char *artist, struct mpdwrapper *mpd);
void screen_library_populate_songs(struct screen_library *screen, char *artist, char *album, struct mpdwrapper *mpd);

void screen_library_select(struct screen_library *screen, int index);
void screen_library_select_prev(struct screen_library *screen);
void screen_library_select_next(struct screen_library *screen);
void screen_library_select_top_visible(struct screen_library *screen);
void screen_library_select_bottom_visible(struct screen_library *screen);
void screen_library_select_middle_visible(struct screen_library *screen);

void screen_library_scroll_page_up(struct screen_library *screen);
void screen_library_scroll_page_down(struct screen_library *screen);

void screen_library_next_view(struct screen_library *screen, struct mpdwrapper *mpd);
void screen_library_prev_view(struct screen_library *screen);

void screen_library_draw(struct screen_library *screen);

#endif /* SCREEN_LIBRARY_H */
