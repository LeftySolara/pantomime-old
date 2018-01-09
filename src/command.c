/******************************************************************************
 * command.c : functions and structs for user input commands
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

#include <ncurses.h>
#include "command.h"

#define KEY_CTRL(x) ((x) & 0x1f)

static struct command_def cmds[] = {

    {CMD_NULL, {0, 0, 0}, "Null", "Null command"},

    {CMD_QUIT, {'q', 'Q', KEY_CTRL('c')}, "Quit", "Quit"},

    {CMD_PAUSE, {'p', 'P', 0}, "Pause", "Toggle pause"},

    {CMD_STOP, {'s', 'S', 0}, "Stop", "Stop the currently playing song"},

    {CMD_VOL_UP, {KEY_RIGHT, 0, 0}, "Volume up", "Increase the volume by 1%"},

    {CMD_VOL_DOWN, {KEY_LEFT, 0, 0}, "Volume down", "Decrease the volume by 1%"},
};

/* Find a command based on the given input key */
enum command find_key_command(int key)
{
    if (key == 0)
        return CMD_NULL;

    for (int i = 0; i < NUM_CMDS; ++i) {
        for (int j = 0; j < MAX_KEYS; ++j) {
            if (key == cmds[i].keys[j])
                return cmds[i].cmd;
        }
    }

    return CMD_NULL;
}
