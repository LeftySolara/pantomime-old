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

#define MODES_LABEL_LENGTH 6
#define PROGRESS_LABEL_LENGTH 16

struct status_bar *status_bar_init()
{
    struct status_bar *status_bar = malloc(sizeof(*status_bar));

    status_bar->win = newwin(2, COLS, LINES - 2, 0);
    status_bar->song_label = NULL;

    status_bar->modes_label = malloc(MODES_LABEL_LENGTH * sizeof(char));
    memset(status_bar->modes_label, '-', MODES_LABEL_LENGTH - 1);

    status_bar->progress_label = malloc(PROGRESS_LABEL_LENGTH * sizeof(char));
    snprintf(status_bar->progress_label, strlen("[00:00]")+1, "[00:00]");

    return status_bar;
}

void status_bar_free(struct status_bar *bar)
{
    delwin(bar->win);
    free(bar->modes_label);
    free(bar->progress_label);
    free(bar->song_label);
    free(bar);
}

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
void draw_statusbar(struct status_bar *status_bar, struct mpdwrapper *mpd)
{
    if (mpd->state == MPD_STATE_UNKNOWN)
        return;

    double song_length = get_current_song_duration(mpd);
    double time_elapsed = get_current_song_elapsed(mpd);
    double width = getmaxx(status_bar->win);

    /* Number of seconds that must elapse for the bar to progress. */
    double secs_per_tick = song_length / width;
    /* Number of spaces to move for every tick. */
    double tick_size = (width / song_length) + 1;
    /* Number of times the bar has moved. */
    double ticks_elapsed = time_elapsed / tick_size;

    werase(status_bar->win);

    /* Draw the progress bar. */
    if (mpd->state != MPD_STATE_STOP) {
        wattr_on(status_bar->win, A_BOLD, NULL);
        whline(status_bar->win, '=', (tick_size * ticks_elapsed) / secs_per_tick);
        mvwaddch(status_bar->win, 0, (tick_size *ticks_elapsed) / secs_per_tick, '>');
        wattr_off(status_bar->win, A_BOLD, NULL);
    }

    /* Print the current player status */
    if (mpd->state == MPD_STATE_PLAY || mpd->state == MPD_STATE_PAUSE) {
        char *title = mpdwrapper_get_song_tag(mpd->current_song, MPD_TAG_TITLE);
        char *artist = mpdwrapper_get_song_tag(mpd->current_song, MPD_TAG_ARTIST);

        status_bar->song_label = create_label_song(status_bar->song_label, title, artist);
        status_bar->progress_label = create_label_progress(status_bar->progress_label, (unsigned int)time_elapsed, (unsigned int)song_length);

        mvwaddstr(status_bar->win, 1, width - strlen(status_bar->progress_label), status_bar->progress_label);
        mvwaddstr(status_bar->win, 1, 0, status_bar->song_label);

        free(title);
        free(artist);
    }

    status_bar->modes_label = create_label_modes(status_bar->modes_label, mpd->status);
    mvwaddnstr(status_bar->win, 1,
               width - strlen(status_bar->modes_label) - strlen(status_bar->progress_label) - 1,
               status_bar->modes_label, 5);

    int volume = mpd_status_get_volume(mpd->status);
    mvwprintw(status_bar->win, 1,
              width - strlen(status_bar->modes_label) - strlen(status_bar->progress_label) - strlen("100%%") - 1,
              "%d%%", volume);

    wnoutrefresh(status_bar->win);
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
    const int mode_cnt = MODES_LABEL_LENGTH - 1;
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

/**
 * @brief Creates a label for the current time elapsed.
 * 
 * @param buffer The char buffer for storing the label.
 * @param time_elapdes The number of seconds the song has been playing.
 * #param song_length The length of the song in seconds,
 * @return A label representing time elapsed for the playing song.
 */
char *create_label_progress(char *buffer, unsigned int time_elapsed, unsigned int song_length)
{
    unsigned int total_minutes = song_length / 60;
    unsigned int total_seconds = song_length % 60;
    unsigned int elapsed_minutes = time_elapsed / 60;
    unsigned int elapsed_seconds = time_elapsed % 60;

    buffer = realloc(buffer, PROGRESS_LABEL_LENGTH);
    snprintf(buffer, PROGRESS_LABEL_LENGTH, "%d:%02d / %d:%02d",
            elapsed_minutes, elapsed_seconds, total_minutes, total_seconds);

    return buffer;
}

/**
 * @brief Creates a label for the currently playing song, formatted ARTIST - TITLE.
 * 
 * @param buffer The char buffer for storing the label.
 * @param title The title of the song.
 * @param artist The song's artist.
 * @return A label representing the currently playing song.
 */
char *create_label_song(char *buffer, const char *title, const char *artist)
{
    const size_t label_len = strlen(artist) + strlen(title) + strlen(" - ") + 1;

    buffer = realloc(buffer, label_len * sizeof(char));
    snprintf(buffer, label_len, "%s - %s", artist, title);

    return buffer;
}