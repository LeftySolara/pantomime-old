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
 * @brief Creates a new connection to an MPD server.
 * 
 * @param host The MPD server host to connect to. Defaults to "localhost".
 * @param port The port MPD is running on. Defaults to 6600.
 * @param timeout The MPD timeout. Defaults to 30000ms.
 * @return Pointer to an mpdwrapper struct.
 */
struct mpdwrapper *mpdwrapper_new(const char *host, int port, int timeout)
{
    struct mpdwrapper *mpd = malloc(sizeof(*mpd));
    if (!mpd)
        return NULL;

    mpdwrapper_initialize(mpd, host, port, timeout);

    return mpd;
}

void mpdwrapper_initialize(struct mpdwrapper *mpd, const char *host, int port, int timeout)
{
    mpd->connection = mpd_connection_new(host, port, timeout);
    mpd->status = mpd_run_status(mpd->connection);
    mpd->current_song = mpd_run_current_song(mpd->connection);
    mpd->queue = songlist_new();
    mpd->state = mpd_status_get_state(mpd->status);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    mpdwrapper_fetch_queue(mpd);
}

/**
 * @brief Frees memory allocated by an mpdwrapper.
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
 */
void mpdwrapper_refresh(struct mpdwrapper *mpd)
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
int mpdwrapper_update_db(struct mpdwrapper *mpd)
{
    int rc = mpd_run_update(mpd->connection, NULL);
    mpd->last_error = mpd_connection_get_error(mpd->connection);

    return rc;
}

struct mpd_status *mpdwrapper_get_status(struct mpdwrapper *mpd)
{
    return mpd->status;
}

/*
 * TEMP:
 * This function only exists to be a placeholder until the playlist view
 * gets rewritten. Once that happens this can go away. The plan is for
 * certain other modules to not have direct access to the play queue.
 */
struct songlist *mpdwrapper_get_queue(struct mpdwrapper *mpd)
{
    return mpd->queue;
}

/**
 * @brief Determines if MPD is currently playing a song.
 * 
 * @param mpd The MPD connection to query.
 * @return bool true if a song is playing, false otherwise.
 */
bool mpdwrapper_is_playing(struct mpdwrapper *mpd)
{
    return mpd->state == MPD_STATE_PLAY;
}

/**
 * @brief Determines if MPD playback is paused.
 * 
 * @param mpd The MPD connection to query.
 * @return bool true if a song is paused, false otherwise.
 */
bool mpdwrapper_is_paused(struct mpdwrapper *mpd)
{
    return mpd->state == MPD_STATE_PAUSE;
}

/**
 * @brief Determines if MPD playback is stopped.
 * 
 * @param mpd The MPD connection to query.
 * @return bool true if no song is playing, false otherwise.
 */
bool mpdwrapper_is_stopped(struct mpdwrapper *mpd)
{
    return mpd->state == MPD_STATE_STOP;
}

/**
 * @brief Determines if MPD playback is in a valid, known state.
 * 
 * @param mpd The MPD connection to query.
 * @return bool true if playback state can be determined, false otherwise.
 */
bool mpdwrapper_has_valid_state(struct mpdwrapper *mpd)
{
    return mpd->state != MPD_STATE_UNKNOWN;
}

struct mpd_song *mpdwrapper_get_current_song(struct mpdwrapper *mpd)
{
    return mpd->current_song;
}

/**
 * @brief Gets the title of the currently playing song.
 * 
 * @param mpd The mpd connection to parse.
 * @return The title of the currently playing song.
 */
const char *mpdwrapper_get_current_song_title(struct mpdwrapper *mpd)
{
    return mpd_song_get_tag(mpd->current_song, MPD_TAG_TITLE, 0);
}

/**
 * @brief Gets the duration of the currently playing song.
 * 
 * @param mpd The mpd connection to parse.
 * @return The length of the song in seconds, or -1 on error.
 */
int mpdwrapper_get_current_song_duration(struct mpdwrapper *mpd)
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
int mpdwrapper_get_current_song_elapsed(struct mpdwrapper *mpd)
{
    if (mpd->current_song)
        return mpd_status_get_elapsed_time(mpd->status);
    else
        return -1;
}

/**
 * @brief Gets the ID of the currently playing song.
 */
int mpdwrapper_get_current_song_id(struct mpdwrapper *mpd)
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

/**
 * @brief Returns an error message describing the last error encountered by MPD.
 */
char *mpdwrapper_get_last_error_message(struct mpdwrapper *mpd)
{
    return mpd_status_get_error(mpd->status);
}

/**
 * @brief Begins playing the specified song from the queue.
 * 
 * Begins playing the specified song from the beginning.
 * On error, the details can be fetched with mpdwrapper_get_last_error_message().
 * 
 * @param mpd The MPD connection to query.
 * @param pos The position of the song in the queue.
 * 
 * @return bool true on success, or false on error.
 */
bool mpdwrapper_play_queue_pos(struct mpdwrapper *mpd, unsigned pos)
{
    bool success = mpd_run_play_pos(mpd->connection, pos);

    if (!success) {
        mpd->last_error = mpd_connection_get_error(mpd->connection);
        mpd->status = mpd_run_status(mpd->connection);
        mpd->state = mpd_status_get_state(mpd->status);
    }

    return success;
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
 * @brief Allocates memory for a new song node.
 * 
 * @return struct songnode* A new song node.
 */
struct songnode *songnode_new(struct mpd_song *song)
{
    struct songnode *new_node = malloc(sizeof(*new_node));
    if (!new_node)
        return NULL;

    songnode_initialize(new_node, song);

    return new_node;
}

void songnode_initialize(struct songnode *node, struct mpd_song *song)
{
    node->song = song;
    node->prev = NULL;
    node->next = NULL;
}

/**
 * @brief Frees memory allocated by a songlist node.
 * 
 * @param node The node to free memory from.
 */
void songnode_free(struct songnode *node)
{
    if (node->song)
        mpd_song_free(node->song);

    node->prev = NULL;
    node->next = NULL;
    free(node);
}

/**
 * @brief Allocates memory for a new songlist.
 * 
 * @return A pointer to a new song list, or NULL on error.
 */
struct songlist *songlist_new()
{
    struct songlist *songlist = malloc(sizeof(*songlist));
    if (!songlist)
        return NULL;

    songlist_initialize(songlist);

    return songlist;
}

void songlist_initialize(struct songlist *songlist)
{
    songlist->head = NULL;
    songlist->tail = NULL;
    songlist->size = 0;
}

/**
 * @brief Frees memory allocated by a song list.
 * 
 * @param list The list to free memory from.
 */
void songlist_free(struct songlist *songlist)
{
    songlist_clear(songlist);
    free(songlist);
}

struct songnode *songlist_node_at(struct songlist *songlist, unsigned int index)
{
    if (index >= songlist->size)
        return NULL;

    if (index == 0)
        return songlist->head;
    if (index == songlist->size-1)
        return songlist->tail;

    struct songnode *current = songlist->head;
    for (int i = 0; i < index; ++i)
        current = current->next;

    return current;
}

/**
 * @brief Gets the MPD song at the given index.
 * 
 * @param list The list to find a song from.
 * @param index The place in the list where the song is.
 * 
 * @return Pointer to the MPD song at the requested position, or NULL on error.
 */
struct mpd_song *songlist_at(struct songlist *songlist, unsigned int index)
{
    return songlist_node_at(songlist, index)->song;
}

/**
 * @brief Gets the number of items in a songlist.
 */
int songlist_get_size(struct songlist *songlist)
{
    return songlist->size;
}

/**
 * @brief Adds a song to the end of the list.
 * 
 * @param list The list to append a song to.
 * @param song The song to add to the list. 
 *   If this is NULL, the function does nothing.
 */
void songlist_append(struct songlist *songlist, struct mpd_song *song)
{
    if (!song)
        return;

    struct songnode *new_node = songnode_new(song);

    if (!songlist->head)
        songlist->head = new_node;
    else {
        struct songnode *tmp = songlist->tail;
        tmp->next = new_node;
        new_node->prev = tmp;
    }

    songlist->tail = new_node;
    songlist->size++;
}

/**
 * @brief Removes the song at the given index from a list.
 * 
 * @param list The list to remove a song from.
 * @param index The position of the song to remove.
 */
void songlist_remove(struct songlist *songlist, unsigned int index)
{
    if (index >= songlist->size)
        return;

    if (index == 0) {
        struct songnode *tmp = songlist->head;

        songlist->head = songlist->head->next;
        songlist->head->prev = NULL;
        songnode_free(tmp);
    }
    else if (index == songlist->size-1) {
        struct songnode *tmp = songlist->tail;

        songlist->tail = songlist->tail->prev;
        songlist->tail->next = NULL;
        songnode_free(tmp);
    }
    else {
        struct songnode *tmp = songlist_node_at(songlist, index);

        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        songnode_free(tmp);
    }

    songlist->size--;
}

/**
 * @brief Removes all items from a songlist.
 * 
 * @param list The list to clear.
 */
void songlist_clear(struct songlist *songlist)
{
    struct songnode *current = songlist->head;
    struct songnode *next;

    while (current) {
        next = current->next;
        songnode_free(current);
        current = next;
    }

    songlist->head = NULL;
    songlist->tail = NULL;
    songlist->size = 0;
}
