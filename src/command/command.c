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
#include <stdlib.h>
#include <string.h>

#define KEY_CTRL(x) ((x) & 0x1f)
#define KEY_RETURN 10

static struct command commands[] = {

    {CMD_NULL, {0, 0, 0}, "Null",
    "Null command"},

    {CMD_QUIT, {'q', 'Q', KEY_CTRL('c')}, "Quit",
    "Quit Pantomime"},

    {CMD_DB_UPDATE, {KEY_CTRL('u'), 0, 0}, "Update",
    "Start a music database update"},

    {CMD_SELECT, {' ', 0, 0}, "Select",
    "Select the currently highlighted menu item"},

    {CMD_PLAY, {KEY_RETURN, 0, 0}, "Play",
    "Play the currently selected track"},

    {CMD_PAUSE, {'p', 'P', 0}, "Pause",
    "Toggle pause"},
    
    {CMD_STOP, {'s', 'S', 0}, "Stop",
    "Stop playback"},

    {CMD_SEEK_BACKWARD, {'b', 0, 0}, "Seek backward",
    "Seek backward"},

    {CMD_SEEK_FORWARD, {'f', 0, 0}, "Seek forward",
    "Seek forward"},

    {CMD_PREV_SONG, {'H', 0, 0}, "Previous song",
    "Skip backward to the previous track in the queue"},

    {CMD_NEXT_SONG, {'L', 0, 0}, "Next song",
    "Skip to the next song in the queue"},

    {CMD_REPEAT, {'r', 0, 0}, "Repeat Mode",
    "Toggle repeat mode"},

    {CMD_RANDOM, {'z', 0, 0}, "Random Mode",
    "Toggle random playback of songs in the queue"},

    {CMD_SINGLE, {'y', 0, 0}, "Single Mode",
    "Toggle single mode"},

    {CMD_CONSUME, {'c', 0, 0}, "Consume Mode",
    "Toggle consume mode"},

    {CMD_CROSSFADE, {'x', 0, 0}, "Crossfade",
    "Toggle crossfade"},

    {CMD_VOL_DOWN, {KEY_LEFT, 0, 0}, "Volume down",
    "Decrease the playback volume"},

    {CMD_VOL_UP, {KEY_RIGHT, 0, 0}, "Volume up",
    "Increase the playback volume"},

    {CMD_DELETE, {'d', 0, 0}, "Delete",
    "Deletes a song from the queue"},

    {CMD_CLEAR, {'C', 0, 0}, "Clear Queue",
    "Removes all songs from the queue"},

    {CMD_PANEL_HELP, {'1', KEY_F(1), 0}, "Help",
    "Show the help screen"},

    {CMD_PANEL_QUEUE, {'2', KEY_F(2), 0}, "Queue",
    "Show the queue screen"},

    {CMD_PANEL_LIBRARY, {'3', KEY_F(3), 0}, "Library",
    "Show the library screen"},

    {CMD_CURSOR_DOWN, {KEY_DOWN, 'j', 0}, "Cursor down",
    "Move the cursor down one line"},

    {CMD_CURSOR_UP, {KEY_UP, 'k', 0}, "Cursor up",
    "Move the cursor up one line"},

    {CMD_CURSOR_LEFT, {'h', 0, 0}, "Cursor left",
    "Move the cursor left."},

    {CMD_CURSOR_RIGHT, {'l', 0, 0}, "Cursor right",
    "Move the cursor right."},

    {CMD_CURSOR_PAGE_DOWN, {KEY_NPAGE, 0, 0}, "Page Down",
    "Page down"},

    {CMD_CURSOR_PAGE_UP, {KEY_PPAGE, 0, 0}, "Page Up",
    "Page up"},

    {CMD_CURSOR_BOTTOM, {'J', 0, 0}, "Move to bottom",
    "Move the cursor to the bottom of the screen."},

    {CMD_CURSOR_TOP, {'K', 0, 0}, "Move to top",
    "Move the cursor to the top of the screen"},

    {CMD_CURSOR_MIDDLE, {'M', 0, 0}, "Move to middle",
    "Move the cursor to the middle of the screen"}
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

/**
 * @brief Gets a string representation of the keys mapped to a command.
 */
void get_command_keys(enum command_type cmd, char *buffer)
{
    int *keys = commands[cmd].keys;
    char *key_str = malloc(16 * sizeof(char));

    if (!key_str)
        return;

    key_to_str(keys[0], key_str);
    strcpy (buffer, key_str);

    if (keys[1] != 0) {
        key_to_str(keys[1], key_str);
        strcat(buffer, " ");
        strcat(buffer, key_str);
    }
    if (keys[2] != 0) {
        key_to_str(keys[2], key_str);
        strcat(buffer, " ");
        strcat(buffer, key_str);
    }

    free(key_str);
}

char *get_command_desc(enum command_type cmd)
{
    return commands[cmd].description;
}

/**
 * @brief Creates a string representation for a keypress.
 */
void key_to_str(int key, char *buffer)
{
    char *str;
    switch(key) {
        case 0:
            str = NULL;
            break;
        case KEY_RETURN:
            str = "Enter";
            break;
        case KEY_BACKSPACE:
            str = "Backspace";
            break;
        case KEY_RIGHT:
            str = "Right";
            break;
        case KEY_LEFT:
            str = "Left";
            break;
        case KEY_UP:
            str = "Up";
            break;
        case KEY_DOWN:
            str = "Down";
            break;
        case KEY_PPAGE:
            str = "PageUp";
            break;
        case KEY_NPAGE:
            str = "PageDown";
            break;
        case KEY_HOME:
            str = "Home";
            break;
        case KEY_END:
            str = "End";
            break;
        case KEY_F(1):
            str = "F1";
            break;
        case KEY_F(2):
            str = "F2";
            break;
        case KEY_F(3):
            str = "F3";
            break;
        case KEY_F(4):
            str = "F4";
            break;
        case KEY_F(5):
            str = "F5";
            break;
        case KEY_F(6):
            str = "F6";
            break;
        case KEY_F(7):
            str = "F7";
            break;
        case KEY_F(8):
            str = "F8";
            break;
        case KEY_F(9):
            str = "F9";
            break;
        case KEY_F(10):
            str = "F10";
            break;
        case KEY_F(11):
            str = "F11";
            break;
        case KEY_F(12):
            str = "F12";
            break;
        default:
            str = NULL;
            break;
    }

    int length;
    if (str) {
        length = strlen(str);
        buffer = realloc(buffer, length * sizeof(char));
        sprintf(buffer, str);
    }
    else if (!str && !(key & ~0x1f)) {  /* A CTRL combo was pressed */
        length = strlen("Ctrl-A");
        buffer = realloc(buffer, length * sizeof(char));
        sprintf(buffer, "Ctrl-%c", 'A' + (key & 0x1f) - 1);
    }
    else { /* The key is just one character */
        buffer = realloc(buffer, sizeof(char));
        sprintf(buffer, "%c", key);
    }
}
