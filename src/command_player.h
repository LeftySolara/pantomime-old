/******************************************************************************
 * command_player.c : commands for interacting with MPD
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

#ifndef COMMAND_PLAYER_H
#define COMMAND_PLAYER_H

#include "mpdclient.h"
#include "command.h"

void mpd_pause(struct mpd_connection *connection);
void mpd_stop(struct mpd_connection *connection);
void mpd_vol_up(struct mpd_connection *connection);
void mpd_vol_down(struct mpd_connection *connection);
void cmd_player(enum command cmd, struct mpdclient *mpd);

#endif