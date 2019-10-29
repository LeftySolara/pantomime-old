/*******************************************************************************
 * mpdwrapper.c
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
 * @file mpdwrapper.h
 */

#include "mpdwrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Creates a connection to an MPD server.
 * 
 * @param host The MPD server host to connect to. Defaults to "localhost".
 * @param port The port MPD is running on. Defaults to 6600.
 * @param timeout The MPD timeout. Defaults to 30000ms.
 * @return Pointer to an mpdwrapper struct.
 */
struct mpdwrapper *mpdwrapper_init(const char *host, int port, int timeout)
{
    struct mpdwrapper *mpd = malloc(sizeof(*mpd));

    mpd->connection = mpd_connection_new(host, port, timeout);
    mpd->status = mpd_run_status(mpd->connection);
    mpd->current_song = mpd_run_current_song(mpd->connection);
    mpd->queue = songlist_init();
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    mpdwrapper_fetch_queue(mpd);

    return mpd;
}

/**
 * @brief Frees memory allocated by an mpdwrapper.
 * 
 * @param mpd Pointer to an mpdwrapper struct.
 */
void mpdwrapper_free(struct mpdwrapper *mpd)
{
    if (mpd->current_song)
        mpd_song_free(mpd->current_song);
    if (mpd->status)
        mpd_status_free(mpd->status);
    if (mpd->connection)
        mpd_connection_free(mpd->connection);
    if (mpd->queue)
        songlist_free(mpd->queue);

    free(mpd);
}

/**
 * @brief Fetches the current MPD queue and stores it in a songlist struct.
 * 
 * @param mpd The MPD wrapper to fetch the queue for.
 */
void mpdwrapper_fetch_queue(struct mpdwrapper *mpd)
{
    if (!mpd->connection)
        return;
    if (mpd->queue)
        songlist_clear(mpd->queue);
    
    struct mpd_song *song;
    mpd_send_list_queue_meta(mpd->connection);
    while ((song = mpd_recv_song(mpd->connection)))
        songlist_append(mpd->queue, song);
    
    mpd_response_finish(mpd->connection);
    mpd->queue_version = mpd_status_get_queue_version(mpd->status);
}

/**
 * @brief Removes a song from the play queue. Does not update the queue version.
 * 
 * @param mpd The mpd connection
 * @param pos The position of the song in the queue
 */
void mpdwrapper_delete_from_queue(struct mpdwrapper *mpd, unsigned pos)
{
    mpd_run_delete(mpd->connection, pos);
}

/**
 * @brief Fetches the current state from the MPD server.
 * 
 * @param mpd Pointer to an mpdwrapper struct.
 */
void mpdwrapper_update(struct mpdwrapper *mpd)
{
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    mpd_status_free(mpd->status);
    mpd->status = mpd_run_status(mpd->connection);

    if (mpd->current_song)
        mpd_song_free(mpd->current_song);
    mpd->current_song = mpd_run_current_song(mpd->connection);

    int queue_version = mpd_status_get_queue_version(mpd->status);
    if (mpd->queue_version != queue_version) {
        mpdwrapper_fetch_queue(mpd);
        mpd->queue_version = queue_version; 
    }
}

/**
 * @brief Performs an update of the MPD music database.
 */
int mpdwrapper_db_update(struct mpdwrapper *mpd)
{
    int rc = mpd_run_update(mpd->connection, NULL);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    return rc;
}

/**
 * @brief Gets the title of the currently playing song.
 * 
 * @param mpd The mpd connection to parse.
 * @return The title of the currently playing song.
 */
const char *get_current_song_title(struct mpdwrapper *mpd)
{
    return mpd_song_get_tag(mpd->current_song, MPD_TAG_TITLE, 0);
}

/**
 * @brief Gets the duration of the currently playing song.
 * 
 * @param mpd The mpd connection to parse.
 * @return The length of the song in seconds, or -1 on error.
 */
int get_current_song_duration(struct mpdwrapper *mpd)
{
    if (mpd->current_song)
        return mpd_song_get_duration(mpd->current_song);
    else
        return -1;
}

/**
 * @brief Gets the amount of time elapsed for the currently playing song.
 * 
 * @param mpd The mpd connection to parse.
 * @return The time elapsed in seconds, or -1 on error.
 */
int get_current_song_elapsed(struct mpdwrapper *mpd)
{
    if (mpd->current_song)
        return mpd_status_get_elapsed_time(mpd->status);
    else
        return -1;
}

/**
 * @brief Gets the ID of the currently playing song.
 */
int get_current_song_id(struct mpdwrapper *mpd)
{
    if (mpd->current_song)
        return mpd_song_get_id(mpd->current_song);
    else
        return -1;
}

/**
 * @brief Gets a tag associated with the given song.
 * 
 * This function gets information about a song based on its MPD tags.
 * It allocates enough memory to hold a null-terminated string
 * containing the tag value. The caller is responsible for
 * freeing this memory.
 * 
 * @return A null-terminated string containing the tag value, or NULL on error.
 */
char *mpdwrapper_get_song_tag(struct mpd_song *song, enum mpd_tag_type tag)
{
    if (!song)
        return NULL;

    const char *tag_val = mpd_song_get_tag(song, tag, 0);
    int len = strlen(tag_val) + 1;

    char *buffer = malloc(len * sizeof(char));
    memcpy(buffer, tag_val, len);

    return buffer;
}