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

void draw_queue(WINDOW *win, struct mpdwrapper *mpd)
{
    werase(win);

    int y = 0;

    struct songnode *node = mpd->queue->head;
    while (node) {
        mvwaddstr(win, y++, 0, mpd_song_get_tag(node->song, MPD_TAG_TITLE, 0));
        node = node->next;
    }

    wnoutrefresh(win);
}