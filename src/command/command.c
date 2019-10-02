/*******************************************************************************
 * command.c
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
 * @file command.h
 */

#include "command.h"
#include <ncurses.h>

#define KEY_CTRL(x) ((x) & 0x1f)

static struct command commands[] = {
    {CMD_NULL, {0, 0, 0}, "Null", "Null command"},
    {CMD_QUIT, {'q', 'Q', KEY_CTRL('c')}, "Quit", "Quit Pantomime"},
    {CMD_PAUSE, {'p', 'P', 0}, "Pause", "Toggle pause"},
    {CMD_STOP, {'s', 'S', 0}, "Stop", "Stop playback"},
    {CMD_PANEL_HELP, {'1', KEY_F(1), 0}, "Help", "Show the help screen"},
    {CMD_PANEL_QUEUE, {'2', KEY_F(2), 0}, "Queue", "Show the queue screen"}
};

/**
 * @brief Finds the command mapped to the given key.
 * 
 * @param key The key that was pressed by the user.
 * @return The command mapped to the key.
 */
enum command_type find_key_command(int key)
{
    if (key == 0)
        return CMD_NULL;
    
    for (int i = 0; i < NUM_CMDS; ++i) {        /* Search each command. */
        for (int j = 0; j < MAX_KEYS; ++j) {    /* Search each key mapped to the command. */
            if (key == commands[i].keys[j])
                return commands[i].cmd;
        }
    }

    return CMD_NULL;
}