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

#include <stdlib.h>
#include <string.h>

/**
 * @brief Draws the status bar at the bottom of the screen.
 * 
 * By default, the currently playing track will be displayed under the
 * progress bar. If a status message needs to be shown to the user, then
 * that message will be printed temporarily instead.
 * 
 * @param win The ncurses window to draw on.
 * @param mpd The mpd connection to parse data from.
 * @param status_buf Buffer to hold the message printed in the status bar.
 */
void draw_statusbar(WINDOW *win, struct mpdwrapper *mpd,
                    char *status_buf, char *modes_buf)
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
    if (mpd->state == MPD_STATE_PLAY || mpd->state == MPD_STATE_PAUSE)
        create_label_song(status_buf, mpd->current_song);

    create_label_modes(modes_buf, mpd->status);

    mvwaddstr(win, 1, 0, status_buf);
    mvwaddnstr(win, 1, width - strlen(modes_buf), modes_buf, 5);

    wnoutrefresh(win);
}

/**
 * @brief Creates a label for the current play mode.
 * 
 * MPD has five playback modes: repeat, random, single, consume, and crossfade.
 * This label showes which combination of modes is activated.
 * 
 * @param buffer A buffer to hold the label.
 * @param status The MPD status to parse information from.
 */
char *create_label_modes(char *buffer, struct mpd_status *status)
{
    const int mode_cnt = 5;
    buffer = realloc(buffer, mode_cnt + 1);
    memset(buffer, '-', mode_cnt);

    if (mpd_status_get_repeat(status))
        buffer[0] = 'r';
    if (mpd_status_get_random(status))
        buffer[1] = 'z';
    if (mpd_status_get_single(status))
        buffer[2] = 's';
    if (mpd_status_get_consume(status))
        buffer[3] = 'c';
    if (mpd_status_get_crossfade(status))
        buffer[4] = 'x';

    return buffer;
}

char *create_label_song(char *buffer, struct mpd_song *song)
{
    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    const size_t label_len = strlen(artist) + strlen(title) + strlen(" - ") + 1;

    buffer = realloc(buffer, label_len);
    snprintf(buffer, label_len, "%s - %s", artist, title);

    return buffer;
}