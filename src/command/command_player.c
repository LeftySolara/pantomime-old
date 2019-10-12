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

void toggle_repeat(struct mpdwrapper *mpd, struct status_bar *status_bar)
{
    bool repeat = mpd_status_get_repeat(mpd->status);
    mpd_run_repeat(mpd->connection, !repeat);

    char *notification = !repeat ? "Repeat mode is on" : "Repeat mode is off";
    set_notification(status_bar, notification, 3);
}

void toggle_random(struct mpdwrapper *mpd, struct status_bar *status_bar)
{
    bool random = mpd_status_get_random(mpd->status);
    mpd_run_random(mpd->connection, !random);

    char *notification = !random ? "Random mode is on" : "Random mode is off";
    set_notification(status_bar, notification, 3);
}

void toggle_single(struct mpdwrapper *mpd, struct status_bar *status_bar)
{
    bool single = mpd_status_get_single(mpd->status);
    mpd_run_single(mpd->connection, !single);

    char *notification = !single ? "Single mode is on" : "Single mode is off";
    set_notification(status_bar, notification, 3);
}

void toggle_consume(struct mpdwrapper *mpd, struct status_bar *status_bar)
{
    bool consume = mpd_status_get_consume(mpd->status);
    mpd_run_consume(mpd->connection, !consume);

    char *notification = !consume ? "Consume mode is on" : "Consume mpde is off";
    set_notification(status_bar, notification, 3);
}

/**
 * @brief Finds the requested player command and executes it.
 * 
 * @param cmd The command to execute.
 * @param mpd The MPD connection to run the command on.
 */
void cmd_player(enum command_type cmd, struct mpdwrapper *mpd, struct status_bar *status_bar)
{
    switch (cmd) {
    case CMD_NULL:
        break;
    case CMD_PAUSE:
        toggle_pause(mpd->connection);
        break;
    case CMD_STOP:
        stop_playback(mpd->connection);
        break;
    case CMD_RANDOM:
        toggle_random(mpd, status_bar);
        break;
    case CMD_REPEAT:
        toggle_repeat(mpd, status_bar);
        break;
    case CMD_SINGLE:
        toggle_single(mpd, status_bar);
        break;
    case CMD_CONSUME:
        toggle_consume(mpd, status_bar);
        break;
    default:
        break;
    }
}