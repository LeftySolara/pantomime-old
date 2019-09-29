/*******************************************************************************
 * statusbar.c
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
 * @file statusbar.h
 * 
 * @brief Functions for drawing and updating the status bar.
 *   The bar includes both a progress bar for the currently playing track
 *   and space for displaying messages to the user.
 */

#include "statusbar.h"

#include <string.h>

/**
 * @brief Draws the status bar at the bottom of the screen.
 * 
 * @param win The ncurses window to draw on.
 * @param mpd The mpd connection to parse data from.
 */
void draw_statusbar(WINDOW *win, struct mpdwrapper *mpd)
{
    if (mpd->state == MPD_STATE_UNKNOWN)
        return;

    double song_length = get_current_song_duration(mpd);
    double time_elapsed = get_current_song_elapsed(mpd);
    double width = getmaxx(win);

    /* Number of seconds that must elapse for the bar to progress. */
    double secs_per_tick = song_length / width;
    /* Number of spaces to move for every tick. */
    double tick_size = (width / song_length) + 1;
    /* Number of times the bar has moved. */
    double ticks_elapsed = time_elapsed / tick_size;

    werase(win);

    /* Draw the progress bar. */
    if (mpd->state != MPD_STATE_STOP) {
        wattr_on(win, A_BOLD, NULL);
        whline(win, '=', (tick_size * ticks_elapsed) / secs_per_tick);
        mvwaddch(win, 0, (tick_size *ticks_elapsed) / secs_per_tick, '>');
        wattr_off(win, A_BOLD, NULL);
    }

    /* Print the current player status */
    mvwaddstr(win, 1, 0, "Song Title");
    mvwaddstr(win, 1, width - strlen("-----"), "-----");

    wnoutrefresh(win);
}