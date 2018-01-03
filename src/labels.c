/******************************************************************************
 * labels.c : functions for creating human-readable strings for MPD data
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
#include <stdlib.h>
#include <stdio.h>
#include "mpdclient.h"

/* Create a label in the form "artist - title" */
char *create_label_song(char *buffer, struct mpd_song *song)
{
    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    const size_t label_size = strlen(artist) + strlen(title) + strlen(" - ") + 1;

    buffer = realloc(buffer, label_size);
    snprintf(buffer, label_size, "%s - %s", artist, title);

    return buffer;
}

/* Create a label for the progress of the currently playing song */
char *create_label_duration(char *buffer)
{
    if (mpdclient->state == MPD_STATE_UNKNOWN || mpdclient->state == MPD_STATE_STOP)
        return NULL;

    const size_t label_size = strlen("123:45 / 123:45") + 1;
    unsigned int total_time = mpd_status_get_total_time(mpdclient->status);
    unsigned int elapsed_time = mpd_status_get_elapsed_time(mpdclient->status);

    unsigned int total_minutes = total_time / 60;
    unsigned int total_seconds = total_time % 60;
    unsigned int elapsed_minutes = elapsed_time / 60;
    unsigned int elapsed_seconds = elapsed_time % 60;

    buffer = realloc(buffer, label_size);
    snprintf(buffer, label_size, "%d:%02d / %d:%02d",
             elapsed_minutes, elapsed_seconds, total_minutes, total_seconds);

    return buffer;

}

/* Create a label for the number of songs in the queue */
char *create_label_queue(char *buffer)
{
    /* account for max number of digits in an unsigned int and a null character */
    const size_t label_size = strlen("songs in queue") + 12;
    unsigned int queue_length = mpd_status_get_queue_length(mpdclient->status);

    buffer = realloc(buffer, label_size);
    snprintf(buffer, label_size, "%u songs in queue", queue_length);

    return buffer;
}

/* Create a label for MPD playback modes */
char *create_label_modes(char *buffer)
{
    const int mode_count = 4; /* mpd has four modes built in */
    buffer = realloc(buffer, mode_count + 1);
    memset(buffer, '-', mode_count);

    if (mpd_status_get_repeat(mpdclient->status))
        buffer[0] = 'r';
    if (mpd_status_get_random(mpdclient->status))
        buffer[1] = 'z';
    if (mpd_status_get_single(mpdclient->status))
        buffer[2] = 's';
    if (mpd_status_get_consume(mpdclient->status))
        buffer[3] = 'c';

    return buffer;
}

char *create_label_volume(char *buffer)
{
    const size_t label_size = strlen("Volume: 100%") + 1;
    unsigned int volume = mpd_status_get_volume(mpdclient->status);
    buffer = realloc(buffer, label_size);
    snprintf(buffer, label_size, "Volume: %d%%", volume);

    return buffer;
}
