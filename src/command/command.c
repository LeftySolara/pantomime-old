/*******************************************************************************
 * command.c - Interface for user input commands
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

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

#define KEY_CTRL(x) ((x)&0x1f)
#define KEY_RETURN 10

static struct command commands[] = {

    {CMD_NULL, {0, 0, 0}, "Null", "Null command. Does Nothing."},

    {CMD_QUIT, {'q', 'Q', KEY_CTRL('c')}, "Quit", "Quit Pantomime"}};

/**
 * @brief Find the command mapped to a given key.
 *
 * @param key The key to look up.
 *
 * @return The command mapped to the key.
 */
enum command_type find_key_command(int key)
{
    if (key == 0) {
        return CMD_NULL;
    }

    for (int i = 0; i < NUM_COMMANDS; ++i) { /* Search each command. */
        for (int j = 0; j < MAX_KEYS; ++j) { /* Search each key mapped to the command. */
            if (key == commands[i].keys[j]) {
                return commands[i].type;
            }
        }
    }
    return CMD_NULL;
}

/**
 * @brief Get a string representation of the keys mapped to a command.
 *
 * @param command_type The type of command to look up.
 *
 * @return A string representation of the keys mapped to the command.
 *
 * This function fetches the string representation of a command. Space for this
 * string is allocated as if malloc() had been called. To avoid memory leaks,
 * the pointer returned from this function must be passed to free().
 */
char *get_command_keys(enum command_type command_type)
{
    int *keys = commands[command_type].keys;
    /* "Backspace" is the longest key name and there can only be a maximum of 3 keys mapped. */
    char *buffer = malloc(sizeof(char) * strlen("Backspace") * 3);

    if (!buffer) {
        perror("malloc");
        return NULL;
    }

    char *key_str = key_to_str(keys[0]);
    strcpy(buffer, key_str);
    free(key_str);

    if (keys[1] != 0) {
        key_str = key_to_str(keys[1]);
        strcat(buffer, " ");
        strcat(buffer, key_str);
        free(key_str);
    }
    if (keys[2] != 0) {
        key_str = key_to_str(keys[2]);
        strcat(buffer, " ");
        strcat(buffer, key_str);
        free(key_str);
    }

    return buffer;
}

/**
 * @brief Return the description of a command.
 */
char *get_command_description(enum command_type command_type)
{
    return commands[command_type].description;
}

/**
 * @brief Return the string representation of a key.
 *
 * @param key The key to look up.
 *
 * This function fetches the string representation of a key. Space for this
 * string is allocated as if malloc() had been called. To avoid memory leaks,
 * the pointer returned from this function must be passed to free().
 */
char *key_to_str(int key)
{
    char *key_name;
    switch (key) {
        case 0:
            key_name = NULL;
            break;
        case KEY_RETURN:
            key_name = "Enter";
            break;
        case KEY_BACKSPACE:
            key_name = "Backspace";
            break;
        case KEY_RIGHT:
            key_name = "Right";
            break;
        case KEY_LEFT:
            key_name = "Left";
            break;
        case KEY_UP:
            key_name = "Up";
            break;
        case KEY_DOWN:
            key_name = "Down";
            break;
        case KEY_PPAGE:
            key_name = "PageUp";
            break;
        case KEY_NPAGE:
            key_name = "PageDown";
            break;
        case KEY_HOME:
            key_name = "Home";
            break;
        case KEY_END:
            key_name = "End";
            break;
        case KEY_F(1):
            key_name = "F1";
            break;
        case KEY_F(2):
            key_name = "F2";
            break;
        case KEY_F(3):
            key_name = "F3";
            break;
        case KEY_F(4):
            key_name = "F4";
            break;
        case KEY_F(5):
            key_name = "F5";
            break;
        case KEY_F(6):
            key_name = "F6";
            break;
        case KEY_F(7):
            key_name = "F7";
            break;
        case KEY_F(8):
            key_name = "F8";
            break;
        case KEY_F(9):
            key_name = "F9";
            break;
        case KEY_F(10):
            key_name = "F10";
            break;
        case KEY_F(11):
            key_name = "F11";
            break;
        case KEY_F(12):
            key_name = "F12";
            break;
        default:
            key_name = NULL;
            break;
    }

    char *buffer;
    int key_name_len;
    if (key_name) {
        key_name_len = strlen(key_name);
        buffer = malloc(sizeof(char) * key_name_len);
        snprintf(buffer, key_name_len, "%s", key_name);
    }
    else if (!key_name && !(key & ~0x1f)) { /* A CTRL combo was pressed. */
        key_name_len = strlen("Ctrl-A");
        buffer = malloc(sizeof(char) * key_name_len);
        snprintf(buffer, key_name_len + 1, "Ctrl-%c", 'A' + (key & 0x1f) - 1);
    }
    else { /* The key is just one character. */
        buffer = malloc(sizeof(char));
        snprintf(buffer, 2, "%c", key);
    }

    return buffer;
}
