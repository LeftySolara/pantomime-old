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

void cmd_add_artist(struct screen_library *screen, struct statusbar *statusbar, struct mpdwrapper *mpd)
{
    char *artist = screen->artist_list_view->selected->text;

    if (mpdwrapper_add_artist(mpd, artist))
        statusbar_set_notification(statusbar, "Added songs by artist to queue", 3);
    else
        statusbar_set_notification(statusbar, "Unable to add artist's songs to queue", 3);
}

void cmd_add_album(struct screen_library *screen, struct statusbar *statusbar, struct mpdwrapper *mpd)
{
    char *artist = screen->artist_list_view->selected->text;
    char *album = screen->album_list_view->selected->text;

    if (mpdwrapper_add_album(mpd, artist, album))
        statusbar_set_notification(statusbar, "Added songs from album to queue", 3);
    else
        statusbar_set_notification(statusbar, "Unable to add album's songs to queue", 3);
}

void cmd_add_song(struct screen_library *screen, struct statusbar *statusbar, struct mpdwrapper *mpd)
{
    char *artist = screen->artist_list_view->selected->text;
    char *album = screen->album_list_view->selected->text;
    char *song = screen->song_list_view->selected->text;

    if (mpdwrapper_add_song(mpd, artist, album, song))
        statusbar_set_notification(statusbar, "Added song to queue", 3);
    else
        statusbar_set_notification(statusbar, "Unable to add song to queue", 3);
}

void cmd_add_to_queue(struct screen_library *screen, struct statusbar *statusbar, struct mpdwrapper *mpd)
{
    if (screen->visible_view == screen->artist_list_view)
        cmd_add_artist(screen, statusbar, mpd);
    else if (screen->visible_view == screen->album_list_view)
        cmd_add_album(screen, statusbar, mpd);
    else
        cmd_add_song(screen, statusbar, mpd);

    mpdwrapper_refresh(mpd);
}

void cmd_library(enum command_type cmd, struct screen_library *screen, struct statusbar *statusbar, struct mpdwrapper *mpd)
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
    case CMD_SELECT:
        cmd_add_to_queue(screen, statusbar, mpd);
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
