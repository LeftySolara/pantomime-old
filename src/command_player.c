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

#include "command_player.h"

void mpd_pause(struct mpd_connection *connection)
{
    mpd_run_toggle_pause(connection);
}

void mpd_stop(struct mpd_connection *connection)
{
    mpd_run_stop(connection);
}

void mpd_vol_up(struct mpd_connection *connection)
{
    mpd_run_change_volume(connection, 1);
}

void mpd_vol_down(struct mpd_connection *connection)
{
    mpd_run_change_volume(connection, -1);
}

void cmd_player(enum command cmd, struct mpdclient *mpd)
{
    switch(cmd) {
    case CMD_NULL:
        break;
    case CMD_PAUSE:
        mpd_pause(mpd->connection);
        break;
    case CMD_STOP:
        mpd_stop(mpd->connection);
        break;
    case CMD_VOL_UP:
        mpd_vol_up(mpd->connection);
        break;
    case CMD_VOL_DOWN:
        mpd_vol_down(mpd->connection);
        break;
    default:
        break;
    }
}
