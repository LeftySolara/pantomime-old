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

void move_cursor_down(struct queue_menu_list *list)
{
    if (!list || list->size == 0)
        return;
    
    struct queue_menu_item *current = list->selected;
    if (current->next) {
        if (current == list->bottom_visible) {
            list->top_visible = list->top_visible->next;
            list->bottom_visible = list->bottom_visible->next;
        }
        list->selected = current->next;
    }
}

void move_cursor_up(struct queue_menu_list *list)
{
    if (!list || list->size == 0)
        return;
    
    struct queue_menu_item *current = list->selected;
    if (current->prev) {
        if (current == list->top_visible) {
            list->top_visible = list->top_visible->prev;
            list->bottom_visible = list->bottom_visible->prev;
        }
        list->selected = current->prev;
    }
}

void move_cursor_page_down(struct queue_menu_list *list, WINDOW *win)
{
    if (!list || list->size == 0)
        return;
    
    list->selected = list->bottom_visible;
    if (!list->selected->next)
        return;

    list->selected = list->selected->next;
    list->top_visible = list->selected;
    queue_menu_list_find_bottom(list, win);
}

void move_cursor_page_up(struct queue_menu_list *list, WINDOW *win)
{
    if (!list || list->size == 0)
        return;
    
    int num_visible = getmaxy(win) - 2;
    for (int i = 0; i <= num_visible; ++ i)
        move_cursor_up(list);
}

void move_cursor_middle(struct queue_menu_list *list, WINDOW *win)
{
    if (!list || list->size == 0)
        return;

    int num_visible = getmaxy(win) - 2;
    int limit = num_visible / 2;

    list->selected = list->top_visible;
    for (int i = 0; i < limit; ++i)
        move_cursor_down(list);
}

void cmd_queue(enum command_type cmd, struct mpdwrapper *mpd, struct ui *ui)
{
    switch (cmd) {
    case CMD_NULL:
        break;
    case CMD_CURSOR_DOWN:
        pmenu_select_next(ui->queue_menu);
        break;
    case CMD_CURSOR_UP:
        pmenu_select_prev(ui->queue_menu);
        break;
    case CMD_CURSOR_PAGE_DOWN:
        pmenu_scroll_page_down(ui->queue_menu);
        break;
    case CMD_CURSOR_PAGE_UP:
        pmenu_scroll_page_up(ui->queue_menu);
        break;
    case CMD_PLAY:
        mpd_run_play_pos(mpd->connection, ui->queue_menu->selected_pos);
        break;
    case CMD_CURSOR_BOTTOM:
        pmenu_select_bottom_visible(ui->queue_menu);
        break;
    case CMD_CURSOR_TOP:
        pmenu_select_top_visible(ui->queue_menu);
        break;
    case CMD_CURSOR_MIDDLE:
        pmenu_select_middle_visible(ui->queue_menu);
        break;
    default:
        break;
    }
}