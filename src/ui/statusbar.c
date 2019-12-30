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


/**
 * @brief Allocates memory for a new status bar.
 */
struct statusbar *statusbar_new()
{
    struct statusbar *statusbar = malloc(sizeof(*statusbar));
    if (!statusbar)
        return NULL;

    statusbar_initialize(statusbar);

    return statusbar;
}

void statusbar_initialize(struct statusbar *statusbar)
{
    statusbar->win = newwin(2, COLS, LINES - 2, 0);
    statusbar->song_label = NULL;

    statusbar->modes_label = malloc(MODES_LABEL_LENGTH * sizeof(char));
    memset(statusbar->modes_label, '-', MODES_LABEL_LENGTH - 1);

    statusbar->progress_label = malloc(PROGRESS_LABEL_LENGTH * sizeof(char));
    snprintf(statusbar->progress_label, strlen("[00:00]")+1, "[00:00]");
}

void statusbar_free(struct statusbar *statusbar)
{
    delwin(statusbar->win);
    free(statusbar->modes_label);
    free(statusbar->progress_label);
    free(statusbar->song_label);
    free(statusbar->notification);
    free(statusbar);
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
 */
void statusbar_draw(struct statusbar *statusbar, struct mpdwrapper *mpd)
{
    enum mpd_state state = mpdwrapper_get_state(mpd);
    struct mpd_status *status = mpdwrapper_get_status(mpd);

    if (state == MPD_STATE_UNKNOWN)
        return;

    werase(statusbar->win);
    statusbar_draw_modes(statusbar, status);
    statusbar_draw_volume(statusbar, status);

    if (state != MPD_STATE_STOP) {
        double song_length = mpdwrapper_get_current_song_duration(mpd);
        double time_elapsed = mpdwrapper_get_current_song_elapsed(mpd);

        statusbar_draw_progress_bar(statusbar, time_elapsed, song_length);
        statusbar_draw_progress_label(statusbar, time_elapsed, song_length);
    }

    /* Either the song or a notification is displayed, not both. */
    if (statusbar->notification && time(NULL) <= statusbar->notify_end)
        statusbar_draw_notification(statusbar);
    else if (state == MPD_STATE_PLAY || state == MPD_STATE_PAUSE)
        statusbar_draw_song_label(statusbar, mpdwrapper_get_current_song(mpd));

    wnoutrefresh(statusbar->win);
}

/**
 * @brief Draws the playback modes label on the status bar.
 */
void statusbar_draw_modes(struct statusbar *statusbar, struct mpd_status *mpd_status)
{
    statusbar->modes_label = statusbar_create_label_modes(statusbar->modes_label, mpd_status);

    int width = getmaxx(statusbar->win);
    int begin_x = width - strlen(statusbar->modes_label) - strlen(statusbar->progress_label) - 1;

    mvwaddnstr(statusbar->win, 1, begin_x, statusbar->modes_label, 5);
}

/**
 * @brief Draws the current MPD volume on the status bar.
 */
void statusbar_draw_volume(struct statusbar *statusbar, struct mpd_status *status)
{
    int volume = mpd_status_get_volume(status);
    int width = getmaxx(statusbar->win);
    int begin_x = width - strlen(statusbar->modes_label) - strlen(statusbar->progress_label) - strlen("100%%") - 1;

    mvwprintw(statusbar->win, 1, begin_x, "%d%%", volume);
}

/**
 * @brief Draws the moving progress bar on the status bar area.
 */
void statusbar_draw_progress_bar(struct statusbar *statusbar, unsigned int time_elapsed, unsigned int song_length)
{
    int width = getmaxx(statusbar->win);

    /* Number of seconds that must elapse for the bar to progress. */
    double secs_per_tick = song_length / width;
    /* Number of spaces to move for every tick. */
    double tick_size = (width / song_length) + 1;
    /* Number of times the bar has moved. */
    double ticks_elapsed = time_elapsed / tick_size;

    wmove(statusbar->win, 0, 0);
    wattr_on(statusbar->win, A_BOLD, NULL);
    whline(statusbar->win, '=', (tick_size * ticks_elapsed) / secs_per_tick);
    mvwaddch(statusbar->win, 0, (tick_size * ticks_elapsed) / secs_per_tick, '>');
    wattr_off(statusbar->win, A_BOLD, NULL);
}

/**
 * @brief Draws the current song progress on the status bar.
 */
void statusbar_draw_progress_label(struct statusbar *statusbar, unsigned int time_elapsed, unsigned int song_length)
{
    statusbar->progress_label = statusbar_create_label_progress(statusbar->progress_label, time_elapsed, song_length);
    int width = getmaxx(statusbar->win);

    mvwaddstr(statusbar->win, 1, width - strlen(statusbar->progress_label), statusbar->progress_label);
}

/**
 * @brief Draws the title and artist of the currently playing song in the status bar.
 */
void statusbar_draw_song_label(struct statusbar *statusbar, struct mpd_song *song)
{

    char *title = mpdwrapper_get_song_tag(song, MPD_TAG_TITLE);
    char *artist = mpdwrapper_get_song_tag(song, MPD_TAG_ARTIST);

    statusbar->song_label = statusbar_create_label_song(statusbar->song_label, title, artist);
    mvwaddstr(statusbar->win, 1, 0, statusbar->song_label);

    free(title);
    free(artist);
}

/**
 * @brief Draws the current notification on the status bar.
 */
void statusbar_draw_notification(struct statusbar *statusbar)
{
    wattr_on(statusbar->win, A_BOLD, NULL);
    mvwaddstr(statusbar->win, 1, 0, statusbar->notification);
    wattr_off(statusbar->win, A_BOLD, NULL);
}

/**
 * @brief Sets the notification to display in the status bar.
 * 
 * @param statusbar The status bar to set a notification for.
 * @param msg The message to display.
 * @param duration The number of seconds to display the notification.
 */
void statusbar_set_notification(struct statusbar *statusbar, char *msg, int duration)
{
    statusbar->notification = realloc(statusbar->notification, (strlen(msg) + 1) * sizeof(char));
    sprintf(statusbar->notification, "%s", msg);
    statusbar->notify_end = time(NULL) + duration;
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
char *statusbar_create_label_modes(char *buffer, struct mpd_status *status)
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
char *statusbar_create_label_progress(char *buffer, unsigned int time_elapsed, unsigned int song_length)
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
char *statusbar_create_label_song(char *buffer, const char *title, const char *artist)
{
    const size_t label_len = strlen(artist) + strlen(title) + strlen(" - ") + 1;

    buffer = realloc(buffer, label_len * sizeof(char));
    snprintf(buffer, label_len, "%s - %s", artist, title);

    return buffer;
}
