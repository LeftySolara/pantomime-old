/*******************************************************************************
 * panel_help.c
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
 * @file panel_help.h
 * 
 */

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#include "panel_help.h"

#include <stdlib.h>
#include <string.h>

static enum command_type global_commands[] = {
    CMD_QUIT,
    CMD_PANEL_HELP,
    CMD_PANEL_QUEUE
};

static enum command_type queue_panel_commands[] = {
    CMD_PLAY,
    CMD_PAUSE,
    CMD_STOP,
    CMD_CURSOR_DOWN,
    CMD_CURSOR_UP
};

void draw_help_screen(WINDOW *win)
{
    werase(win);

    int y = 1;

    draw_help_header(win, y++, "Global");
    for (int i = 0; i < ARRAY_LENGTH(global_commands); ++i)
        draw_command_info(win, ++y, global_commands[i]);

    y += 2;
    draw_help_header(win, y++, "Queue Screen");
    for (int i = 0; i < ARRAY_LENGTH(queue_panel_commands); ++i)
        draw_command_info(win, ++y, queue_panel_commands[i]);

    wnoutrefresh(win);
}

/**
 * @brief Prints the section name for a group of commands.
 * 
 * @param win The window to print onto,
 * @param begin_y The y-position of the text.
 * @param header_text The text to print.
 */
void draw_help_header(WINDOW *win, int begin_y, char *header_text)
{
    const int begin_x = 6;  /* Arbitrary */

    wattr_on(win, A_BOLD, NULL);
    mvwaddstr(win, begin_y, begin_x, header_text);
    wattr_off(win, A_BOLD, NULL);

    /* Draw a horizontal rule under the header. */
    wmove(win, begin_y + 1, begin_x);
    for (int i = 0; i < strlen(header_text); ++i)
        waddch(win, ACS_HLINE);
}

/**
 * @brief Prints information about the requested command.
 */
void draw_command_info(WINDOW *win, int begin_y, enum command_type cmd)
{
    if (cmd == CMD_NULL)
        return;
    
    /* The x position of the character that separates the keys and desc. */
    int colon_pos = 17;

    char *desc = get_command_desc(cmd);
    char *keys = malloc(16 * sizeof(char));
    get_command_keys(cmd, keys);

    wmove(win, begin_y, colon_pos - strlen(keys) - 1);
    waddstr(win, keys);
    waddstr(win, " : ");
    waddstr(win, desc);

    if (keys)
        free(keys);
}