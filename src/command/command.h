/*******************************************************************************
 * command.h
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

#ifndef COMMAND_H
#define COMMAND_H

#define MAX_KEYS 3 /* Maximum number of keys a command can be mapped to. */

enum command_type {
    CMD_NULL,
    CMD_QUIT,
    CMD_PLAY,
    CMD_PAUSE,
    CMD_STOP,
    CMD_REPEAT,
    CMD_RANDOM,
    CMD_SINGLE,
    CMD_CONSUME,
    CMD_CROSSFADE,
    CMD_VOL_DOWN,
    CMD_VOL_UP,
    CMD_PANEL_HELP,
    CMD_PANEL_QUEUE,
    CMD_CURSOR_DOWN,
    CMD_CURSOR_UP,
    NUM_CMDS
};

struct command {
    enum command_type cmd;  /** The type of command to execute. */
    int keys[MAX_KEYS];     /** The keys bound to the command. */
    char *name;             /** The name of the command. */
    char *description;      /** Brief description of what the command does. */
};

enum command_type find_key_command(int key);
void get_command_keys(enum command_type cmd, char *buffer);
char *get_command_desc(enum command_type cmd);
void key_to_str(int key, char *buffer);

#endif