/*******************************************************************************
 * command_library.c
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

#include "command_library.h"

void cmd_library(enum command_type cmd, struct ui *ui)
{
    switch (cmd) {
    case CMD_NULL:
        break;
    case CMD_CURSOR_DOWN:
        list_view_select_next(ui->library);
        break;
    case CMD_CURSOR_UP:
        list_view_select_prev(ui->library);
        break;
    case CMD_CURSOR_PAGE_DOWN:
        list_view_scroll_page_down(ui->library);
        break;
    case CMD_CURSOR_PAGE_UP:
        list_view_scroll_page_up(ui->library);
        break;
    case CMD_CURSOR_BOTTOM:
        list_view_select_bottom_visible(ui->library);
        break;
    case CMD_CURSOR_TOP:
        list_view_select_top_visible(ui->library);
        break;
    case CMD_CURSOR_MIDDLE:
        list_view_select_top_visible(ui->library);
        break;
    default:
        break;
    }
}

