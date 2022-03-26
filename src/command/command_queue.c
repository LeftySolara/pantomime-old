/*******************************************************************************
 * command_queue.c
 *******************************************************************************
 * Copyright (C) 2019-2022  Julianne Adams
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

#include <stdlib.h>
#include <string.h>

#include "../ui/statusbar.h"

void queue_remove_selected(struct mpdwrapper *mpd, struct ui *ui)
{
    char *title = ui->queue->selected->title;
    int len_msg = strlen(title) + strlen("Removed '' from play queue") + 1;

    char *msg = malloc(len_msg * sizeof(char));
    snprintf(msg, len_msg, "Removed '%s' from play queue", title);

    mpdwrapper_delete_from_queue(mpd, ui->queue->idx_selected);
    playlist_remove_selected(ui->queue);
    statusbar_set_notification(ui->statusbar, msg, 3);

    free(msg);
}

/* TODO: prompt user to confirm they want to clear the queue. */
void queue_clear(struct mpdwrapper *mpd, struct ui *ui)
{
    char *msg_content = "Queue cleared";
    int len_msg = strlen(msg_content) + 1;

    char *msg = malloc(len_msg * sizeof(char));
    snprintf(msg, len_msg, msg_content);

    mpdwrapper_clear_queue(mpd);
    playlist_clear(ui->queue);
    statusbar_set_notification(ui->statusbar, msg, 3);

    free(msg);
}

void cmd_play_queue_pos(struct mpdwrapper *mpd, struct ui *ui)
{
    bool success = mpdwrapper_play_queue_pos(mpd, ui->queue->idx_selected);

    if (!success) {
        char *error_msg = mpdwrapper_get_last_error_message(mpd);
        statusbar_set_notification(ui->statusbar, error_msg, DEFAULT_NOTIFICATION_LENGTH);
    }
}

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
            cmd_play_queue_pos(mpd, ui);
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
        case CMD_DELETE:
            queue_remove_selected(mpd, ui);
            break;
        case CMD_CLEAR:
            queue_clear(mpd, ui);
            break;
        default:
            break;
    }
}
