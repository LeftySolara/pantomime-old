/*******************************************************************************
 * playlist_view_internal.h
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

#ifndef PLAYLIST_VIEW_INTERNAL_H
#define PLAYLIST_VIEW_INTERNAL_H

#include "list_view.h"
#include "pantomime/playlist_view.h"

struct playlist_view_item {
    struct list_view_item base;

    char *artist; /* The song's artist. */
    char *title;  /* The song's title. */
    char *album;  /* The song's album. */
    int time;     /* Length of the song in seconds. */
    unsigned id;  /* The MPD ID of the song. */
};

struct playlist_view {
    struct list_view base;
};

struct playlist_view_item *playlist_view_item_new(char *artist, char *title, char *album, int time,
                                                  unsigned id);
void playlist_view_item_initialize(struct playlist_view_item *this, char *artist, char *title,
                                   char *album, int time, unsigned id);
void playlist_view_item_free(struct playlist_view_item *this);

void playlist_view_initialize(struct playlist_view *this, int height, int width);

#endif