/*******************************************************************************
 * command_player.h
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
 * @brief Functions that control MPD playback.
 *
 */

#ifndef COMMAND_PLAYER_H
#define COMMAND_PLAYER_H

#include "../mpdwrapper/mpdwrapper.h"
#include "../ui/statusbar.h"
#include "command.h"

void toggle_pause(struct mpd_connection *connection);
void start_playback(int id);
void stop_playback(struct mpd_connection *connection);

void seek_backward(struct mpdwrapper *mpd);
void seek_forward(struct mpdwrapper *mpd);

void prev_song(struct mpdwrapper *mpd, struct statusbar *statusbar);
void next_song(struct mpdwrapper *mpd, struct statusbar *statusbar);

void toggle_repeat(struct mpdwrapper *mpd, struct statusbar *statusbar);
void toggle_random(struct mpdwrapper *mpd, struct statusbar *statusbar);
void toggle_single(struct mpdwrapper *mpd, struct statusbar *statusbar);
void toggle_consume(struct mpdwrapper *mpd, struct statusbar *statusbar);
void toggle_crossfade(struct mpdwrapper *mpd, struct statusbar *statusbar);

void decrease_volume(struct mpd_connection *connection);
void increase_volume(struct mpd_connection *connection);

void cmd_player(enum command_type cmd, struct mpdwrapper *mpd, struct statusbar *statusbar);

#endif
