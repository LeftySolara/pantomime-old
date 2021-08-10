/*******************************************************************************
 * playlist_view.c
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

#include "playlist_view.h"

#include <stdlib.h>

struct playlist_view_item *playlist_view_item_new(char *artist, char *title, char *album, int time,
                                                  unsigned id)
{
    struct playlist_view_item *item = malloc(sizeof(*item));
    if (!item)
        return NULL;

    playlist_view_item_initialize(item, artist, title, album, time, id);

    return item;
}

void playlist_view_item_initialize(struct playlist_view_item *this, char *artist, char *title,
                                   char *album, int time, unsigned id)
{
    list_view_item_initialize((struct list_view_item *)this, NULL);
    this->artist = artist;
    this->title = title;
    this->album = album;
    this->time = time;
    this->id = id;
}

void playlist_view_item_free(struct playlist_view_item *this)
{
    if (!this)
        return;

    free(this->artist);
    free(this->title);
    free(this->album);
    free(this);
}

struct playlist_view *playlist_view_new(int height, int width)
{
    struct playlist_view *view = malloc(sizeof(*view));
    if (!view)
        return NULL;

    playlist_view_initialize(view, height, width);

    return view;
}

void playlist_view_initialize(struct playlist_view *this, int height, int width)
{
    list_view_initialize((struct list_view *)this, height, width);
}

void playlist_view_free(struct playlist_view *this)
{
    free(this);
}