/******************************************************************************
 * command.h : functions and structs for user input commands
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

#ifndef COMMAND_H
#define COMMAND_H

#define MAX_KEYS 3

enum command {
    CMD_NULL,
    CMD_QUIT,
    CMD_PAUSE,
    CMD_STOP,
    CMD_VOL_UP,
    CMD_VOL_DOWN,
    CMD_CURSOR_UP,
    CMD_CURSOR_DOWN,
    NUM_CMDS
};

struct command_def {
    enum command cmd;
    int keys[MAX_KEYS];
    char *name;
    char *description;
};

enum command find_key_command(int key);

#endif
