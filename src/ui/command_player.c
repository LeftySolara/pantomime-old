/*******************************************************************************
 * command_player.c
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
 * @file command_player.h
 * 
 */

#include "command_player.h"

void toggle_pause(struct mpd_connection *connection)
{
    mpd_run_toggle_pause(connection);
}

void stop_playback(struct mpd_connection *connection)
{
    mpd_run_stop(connection);
}

/**
 * @brief Finds the requested player command and executes it.
 * 
 * @param cmd The command to execute.
 * @param mpd The MPD connection to run the command on.
 */
void cmd_player(enum command_type cmd, struct mpdwrapper *mpd)
{
    switch (cmd) {
    case CMD_NULL:
        break;
    case CMD_PAUSE:
        toggle_pause(mpd->connection);
        break;
    case CMD_STOP:
        stop_playback(mpd->connection);
    default:
        break;
    }
}