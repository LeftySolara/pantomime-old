/*******************************************************************************
 * command.h - Interface for user input commands
 *******************************************************************************
 * Copyright (C) 2017-2022  Julianne Adams
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

#ifndef COMMAND_H
#define COMMAND_H

#define MAX_KEYS 3 /* The maximum number of keys a command can be mapped to. */

enum command_type { CMD_NULL, CMD_QUIT, NUM_COMMANDS };

struct command {
    enum command_type type; /** The type of the command. */
    int keys[MAX_KEYS];     /** The keys mapped to the command. */
    char *name;             /** The user-friendly name of the command. */
    char *description;      /** A brief description of whet the command does. */
};

enum command_type find_key_command(int key);

char *get_command_keys(enum command_type command_type);
char *get_command_description(enum command_type command_type);

char *key_to_str(int key);

#endif /* COMMAND_H */
