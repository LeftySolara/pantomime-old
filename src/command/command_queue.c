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

void cmd_queue(enum command_type cmd, struct mpdwrapper *mpd, struct ui *ui)
{
    switch (cmd) {
    case CMD_NULL:
        break;
    case CMD_CURSOR_DOWN:
        playlist_select_next(ui->queue);
        break;
    case CMD_CURSOR_UP:
        playlist_select_prev(ui->queue);
        break;
    case CMD_CURSOR_PAGE_DOWN:
        playlist_scroll_page_down(ui->queue);
        break;
    case CMD_CURSOR_PAGE_UP:
        playlist_scroll_page_up(ui->queue);
        break;
    case CMD_PLAY:
        mpd_run_play_pos(mpd->connection, ui->queue->idx_selected);
        break;
    case CMD_CURSOR_BOTTOM:
        playlist_select_bottom_visible(ui->queue);
        break;
    case CMD_CURSOR_TOP:
        playlist_select_top_visible(ui->queue);
        break;
    case CMD_CURSOR_MIDDLE:
        playlist_select_middle_visible(ui->queue);
        break;
    default:
        break;
    }
}