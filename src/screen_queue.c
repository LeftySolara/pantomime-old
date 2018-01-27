/******************************************************************************
 * screen_queue.c : interface for drawing the MPD queue screen
 * ****************************************************************************
 * Copyright (C) 2017 Jalen Adams
 *
 * Authors: Jalen Adams <leftysolara@gmail.com>
 *
 * This file is part of Pantomime.
 *
 * Pantomime is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pantomime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pantomime.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <string.h>
#include "screen_queue.h"

void draw_track(WINDOW *win, struct tracknode *track, int row, int width, bool selected, bool playing)
{
    int maxx = getmaxx(win);

    if (playing)
        wattr_on(win, A_BOLD, 0);
    if (selected)
        wattr_on(win, A_STANDOUT, 0);

    mvwprintw(win, row, 0, "%s\n", track->artist);
    mvwprintw(win, row, width + 1, " %s\n", track->title);
    mvwprintw(win, row, (width * 2) + 1, " %s\n", track->album);
    mvwprintw(win, row, maxx - 8, " %d:%02d\n",
              track->duration / 60, track->duration % 60);

    if (selected)
        mvwchgat(win, row, 0, -1, A_STANDOUT, 0, NULL);
    wattr_off(win, A_BOLD, 0);
    wattr_off(win, A_STANDOUT, 0);
}

void draw_queue(struct tracklist *list, struct mpd_song *current_song, WINDOW *win)
{
    struct tracknode *node = list->head;
    bool selected;
    bool playing;

    int y, x;
    getmaxyx(win, y, x);
    int width = (x - 8) / 3; /* Width of each field. Track time has fixed width */
    
    wclear(win);
    wattr_on(win, A_BOLD, NULL);
    mvwprintw(win, 0, 0,"Artist\n");
    mvwprintw(win, 0, width + 1, " Title\n");
    mvwprintw(win, 0, (width * 2) + 1, " Album\n");
    mvwprintw(win, 0, x - 8, " Length\n");
    wattr_off(win, A_BOLD, NULL);

    for (int i = 1; i < y; ++i) {
        playing = node->id == mpd_song_get_id(current_song);
        selected = node->id == list->selected->id;

        draw_track(win, node, i, width, selected, playing);
        if (!node->next)
            break;
        else
            node = node->next;
    }
}
