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

void cmd_library(enum command_type cmd, struct screen_library *screen, struct mpdwrapper *mpd)
{
    switch(cmd) {
    case CMD_NULL:
        break;
    case CMD_CURSOR_DOWN:
        screen_library_select_next(screen);
        break;
    case CMD_CURSOR_UP:
        screen_library_select_prev(screen);
        break;
    case CMD_CURSOR_LEFT:
        screen_library_prev_view(screen);
        break;
    case CMD_CURSOR_RIGHT:
        screen_library_next_view(screen, mpd);
        break;
    case CMD_CURSOR_PAGE_DOWN:
        screen_library_scroll_page_down(screen);
        break;
    case CMD_CURSOR_PAGE_UP:
        screen_library_scroll_page_up(screen);
        break;
    case CMD_PLAY:  /* TODO: Implement */
        break;
    case CMD_CURSOR_BOTTOM:
        screen_library_select_bottom_visible(screen);
        break;
    case CMD_CURSOR_TOP:
        screen_library_select_top_visible(screen);
        break;
    case CMD_CURSOR_MIDDLE:
        screen_library_select_middle_visible(screen);
        break;
    default:
        break;
    }
}
