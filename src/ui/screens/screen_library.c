/*******************************************************************************
 * screen_library.c
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

#include "screen_library.h"
#include <stdlib.h>

struct screen_library *screen_library_new(int height, int width)
{
    struct screen_library *screen = malloc(sizeof(*screen));
    if (!screen)
        return NULL;
    
    screen_library_initialize(screen, height, width);

    return screen;
}

void screen_library_initialize(struct screen_library *screen, int height, int width)
{
    screen->artist_list_view = list_view_new(height, width);
}

void screen_library_free(struct screen_library *screen)
{
    list_view_free(screen->artist_list_view);
    free(screen);
}

void screen_library_populate_artists(struct screen_library *screen, struct mpdwrapper *mpd)
{
    struct stringlist *artist_list = mpdwrapper_list_artists(mpd);
    struct list_view *list_view = screen->artist_list_view;
    struct stringlist_item *current = artist_list->head;

    while (current) {
        list_view->lv_ops->lv_append(list_view, current->str);
        current = current->next;
    }

    stringlist_free(artist_list);
}
