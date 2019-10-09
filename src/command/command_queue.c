/*******************************************************************************
 * command_queue.c
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

#include "command_queue.h"

/**
 * @brief Moves the cursor up or down on the queue screen.
 */
void move_cursor(struct queue_menu_list *list, enum direction direction)
{
    struct queue_menu_item *current = list->selected;

    if (direction == UP && current->prev)
        list->selected = current->prev;
    else if (direction == DOWN && current->next)
        list->selected = current->next;
}

void cmd_queue(enum command_type cmd, struct mpdwrapper *mpd,  struct queue_menu_list *list)
{
    switch (cmd) {
    case CMD_NULL:
        break;
    case CMD_CURSOR_DOWN:
        move_cursor(list, DOWN);
        break;
    case CMD_CURSOR_UP:
        move_cursor(list, UP);
        break;
    case CMD_PLAY:
        mpd_run_play_id(mpd->connection, list->selected->id);
        break;
    default:
        break;
    }
}