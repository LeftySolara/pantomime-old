/*******************************************************************************
 * command_player.c
 *******************************************************************************
 * Copyright (C) 2019-2022  Julianne Adams
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

void seek_backward(struct mpdwrapper *mpd)
{
    if (mpd->state != MPD_STATE_PLAY)
        return;

    unsigned int elapsed_time = mpdwrapper_get_current_song_elapsed(mpd);
    if (elapsed_time > 0) {
        mpd_run_seek_pos(mpd->connection, mpd_song_get_pos(mpd->current_song), elapsed_time - 1);
    }
}

void seek_forward(struct mpdwrapper *mpd)
{
    if (mpd->state != MPD_STATE_PLAY)
        return;

    unsigned int elapsed_time = mpdwrapper_get_current_song_elapsed(mpd);
    unsigned int total_time = mpdwrapper_get_current_song_duration(mpd);

    if (elapsed_time != total_time) { /* Song hasn't finished playing */
        mpd_run_seek_pos(mpd->connection, mpd_song_get_pos(mpd->current_song), elapsed_time + 1);
    }
}

void prev_song(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    if (mpd->state == MPD_STATE_STOP)
        statusbar_set_notification(statusbar, "Not playing", 3);
    else
        mpd_run_previous(mpd->connection);
}

void next_song(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    if (mpd->state == MPD_STATE_STOP)
        statusbar_set_notification(statusbar, "Not playing", 3);
    else
        mpd_run_next(mpd->connection);
}

void toggle_repeat(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    bool repeat = mpd_status_get_repeat(mpd->status);
    mpd_run_repeat(mpd->connection, !repeat);

    char *notification = !repeat ? "Repeat mode is on" : "Repeat mode is off";
    statusbar_set_notification(statusbar, notification, 3);
}

void toggle_random(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    bool random = mpd_status_get_random(mpd->status);
    mpd_run_random(mpd->connection, !random);

    char *notification = !random ? "Random mode is on" : "Random mode is off";
    statusbar_set_notification(statusbar, notification, 3);
}

void toggle_single(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    bool single = mpd_status_get_single(mpd->status);
    mpd_run_single(mpd->connection, !single);

    char *notification = !single ? "Single mode is on" : "Single mode is off";
    statusbar_set_notification(statusbar, notification, 3);
}

void toggle_consume(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    bool consume = mpd_status_get_consume(mpd->status);
    mpd_run_consume(mpd->connection, !consume);

    char *notification = !consume ? "Consume mode is on" : "Consume mode is off";
    statusbar_set_notification(statusbar, notification, 3);
}

void toggle_crossfade(struct mpdwrapper *mpd, struct statusbar *statusbar)
{
    unsigned int crossfade = mpd_status_get_crossfade(mpd->status);
    char *notification;

    if (crossfade == 0) {
        mpd_run_crossfade(mpd->connection, 5);
        notification = "Crossfade set to 5 seconds";
    }
    else {
        mpd_run_crossfade(mpd->connection, 0);
        notification = "Crossfade set to 0 seconds";
    }

    statusbar_set_notification(statusbar, notification, 3);
}

void decrease_volume(struct mpd_connection *connection)
{
    mpd_run_change_volume(connection, -2);
}

void increase_volume(struct mpd_connection *connection)
{
    mpd_run_change_volume(connection, 2);
}

/**
 * @brief Finds the requested player command and executes it.
 *
 * @param cmd The command to execute.
 * @param mpd The MPD connection to run the command on.
 */
void cmd_player(enum command_type cmd, struct mpdwrapper *mpd, struct statusbar *statusbar)
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
        case CMD_SEEK_BACKWARD:
            seek_backward(mpd);
            break;
        case CMD_SEEK_FORWARD:
            seek_forward(mpd);
            break;
        case CMD_PREV_SONG:
            prev_song(mpd, statusbar);
            break;
        case CMD_NEXT_SONG:
            next_song(mpd, statusbar);
            break;
        case CMD_RANDOM:
            toggle_random(mpd, statusbar);
            break;
        case CMD_REPEAT:
            toggle_repeat(mpd, statusbar);
            break;
        case CMD_SINGLE:
            toggle_single(mpd, statusbar);
            break;
        case CMD_CONSUME:
            toggle_consume(mpd, statusbar);
            break;
        case CMD_CROSSFADE:
            toggle_crossfade(mpd, statusbar);
            break;
        case CMD_VOL_DOWN:
            decrease_volume(mpd->connection);
            break;
        case CMD_VOL_UP:
            increase_volume(mpd->connection);
            break;
        default:
            break;
    }
}
