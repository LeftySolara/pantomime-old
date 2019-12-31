/*******************************************************************************
 * mpdwrapper.h
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

#ifndef MPDWRAPPER_H
#define MPDWRAPPER_H

#include <mpd/client.h>

struct mpdwrapper;
struct songlist;

struct mpdwrapper *mpdwrapper_new(const char *host, int port, int timeout);
void mpdwrapper_free(struct mpdwrapper *mpd);

void mpdwrapper_delete_from_queue(struct mpdwrapper *mpd, unsigned pos);

void mpdwrapper_refresh(struct mpdwrapper *mpd);
int mpdwrapper_update_db(struct mpdwrapper *mpd);

struct mpd_status *mpdwrapper_get_status(struct mpdwrapper *mpd);
struct songlist *mpdwrapper_get_queue(struct mpdwrapper *mpd);

bool mpdwrapper_is_playing(struct mpdwrapper *mpd);
bool mpdwrapper_is_paused(struct mpdwrapper *mpd);
bool mpdwrapper_is_stopped(struct mpdwrapper *mpd);
bool mpdwrapper_has_valid_state(struct mpdwrapper *mpd);

struct mpd_song *mpdwrapper_get_current_song(struct mpdwrapper *mpd);
const char *mpdwrapper_get_current_song_title(struct mpdwrapper *mpd);
int mpdwrapper_get_current_song_duration(struct mpdwrapper *mpd);
int mpdwrapper_get_current_song_elapsed(struct mpdwrapper *mpd);
int mpdwrapper_get_current_song_id(struct mpdwrapper *mpd);

/* TODO: Make this function internal? */
char *mpdwrapper_get_song_tag(struct mpd_song *song, enum mpd_tag_type tag);

char *mpdwrapper_get_last_error_message(struct mpdwrapper *mpd);

bool mpdwrapper_play_queue_pos(struct mpdwrapper *mpd, unsigned pos);


struct songlist *songlist_new();
void songlist_free(struct songlist *songlist);

struct mpd_song *songlist_at(struct songlist *songlist, unsigned int index);
int songlist_get_size(struct songlist *songlist);

void songlist_append(struct songlist *songlist, struct mpd_song *song);
void songlist_remove(struct songlist *songlist, unsigned int index);
void songlist_clear(struct songlist *songlist);

#endif /* MPDWRAPPER_H */
