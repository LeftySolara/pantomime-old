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
 */
void draw_statusbar(struct status_bar *status_bar, struct mpdwrapper *mpd)
{
    if (mpd->state == MPD_STATE_UNKNOWN)
        return;

    werase(status_bar->win);
    draw_modes(status_bar, mpd->status);
    draw_volume(status_bar, mpd->status);

    if (mpd->state != MPD_STATE_STOP) {
        double song_length = get_current_song_duration(mpd);
        double time_elapsed = get_current_song_elapsed(mpd);

        draw_progress_bar(status_bar, time_elapsed, song_length);
        draw_progress_label(status_bar, time_elapsed, song_length);
    }

    /* Either the song or a notification is displayed, not both. */
    if (status_bar->notification && time(NULL) <= status_bar->notify_end)
        draw_notification(status_bar);
    else if (mpd->state == MPD_STATE_PLAY || mpd->state == MPD_STATE_PAUSE)
        draw_song_label(status_bar, mpd->current_song);

    wnoutrefresh(status_bar->win);
}

/**
 * @brief Draws the playback modes label on the status bar.
 */
void draw_modes(struct status_bar *status_bar, struct mpd_status *mpd_status)
{
    status_bar->modes_label = create_label_modes(status_bar->modes_label, mpd_status);

    int width = getmaxx(status_bar->win);
    int begin_x = width - strlen(status_bar->modes_label) - strlen(status_bar->progress_label) - 1;

    mvwaddnstr(status_bar->win, 1, begin_x, status_bar->modes_label, 5);
}

/**
 * @brief Draws the current MPD volume on the status bar.
 */
void draw_volume(struct status_bar *status_bar, struct mpd_status *status)
{
    int volume = mpd_status_get_volume(status);
    int width = getmaxx(status_bar->win);
    int begin_x = width - strlen(status_bar->modes_label) - strlen(status_bar->progress_label) - strlen("100%%") - 1;

    mvwprintw(status_bar->win, 1, begin_x, "%d%%", volume);
}

/**
 * @brief Draws the moving progress bar on the status bar area.
 */
void draw_progress_bar(struct status_bar *status_bar, unsigned int time_elapsed, unsigned int song_length)
{
    int width = getmaxx(status_bar->win);

    /* Number of seconds that must elapse for the bar to progress. */
    double secs_per_tick = song_length / width;
    /* Number of spaces to move for every tick. */
    double tick_size = (width / song_length) + 1;
    /* Number of times the bar has moved. */
    double ticks_elapsed = time_elapsed / tick_size;

    wmove(status_bar->win, 0, 0);
    wattr_on(status_bar->win, A_BOLD, NULL);
    whline(status_bar->win, '=', (tick_size * ticks_elapsed) / secs_per_tick);
    mvwaddch(status_bar->win, 0, (tick_size * ticks_elapsed) / secs_per_tick, '>');
    wattr_off(status_bar->win, A_BOLD, NULL);
}

/**
 * @brief Draws the current song progress on the status bar.
 */
void draw_progress_label(struct status_bar *status_bar, unsigned int time_elapsed, unsigned int song_length)
{
    status_bar->progress_label = create_label_progress(status_bar->progress_label, time_elapsed, song_length);
    int width = getmaxx(status_bar->win);

    mvwaddstr(status_bar->win, 1, width - strlen(status_bar->progress_label), status_bar->progress_label);
}

/**
 * @brief Draws the title and artist of the currently playing song in the status bar.
 */
void draw_song_label(struct status_bar *status_bar, struct mpd_song *song)
{

    char *title = mpdwrapper_get_song_tag(song, MPD_TAG_TITLE);
    char *artist = mpdwrapper_get_song_tag(song, MPD_TAG_ARTIST);

    status_bar->song_label = create_label_song(status_bar->song_label, title, artist);
    mvwaddstr(status_bar->win, 1, 0, status_bar->song_label);

    free(title);
    free(artist);
}

/**
 * @brief Draws the current notification on the status bar.
 */
void draw_notification(struct status_bar *status_bar)
{
    wattr_on(status_bar->win, A_BOLD, NULL);
    mvwaddstr(status_bar->win, 1, 0, status_bar->notification);
    wattr_off(status_bar->win, A_BOLD, NULL);
}

/**
 * @brief Sets the notification to display in the status bar.
 * 
 * @param status_bar The status bar to set a notification for.
 * @param msg The message to display.
 * @param duration The number of seconds to display the notification.
 */
void set_notification(struct status_bar *status_bar, char *msg, int duration)
{
    status_bar->notification = realloc(status_bar->notification, (strlen(msg) + 1) * sizeof(char));
    sprintf(status_bar->notification, "%s", msg);
    status_bar->notify_end = time(NULL) + duration;
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