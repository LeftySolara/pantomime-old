/*******************************************************************************
 * ui.h
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

#include "ui.h"

#include <locale.h>
#include <stdlib.h>

#include "panel_help.h"

/**
 * @file ui.h
 *
 */

void start_curses()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    refresh();
}

void end_curses()
{
    endwin();
}

/**
 * @brief Create panels with the given width and height.
 *
 * @param num_panels The number of panels to create.
 * @param width The desired width of all the panels.
 * @param height The desired height of all the panels.
 * @return An array of PANEL pointers
 */
PANEL **create_panels(int num_panels, int height, int width)
{
    PANEL **panels = malloc(num_panels * sizeof(PANEL *));
    WINDOW *win;

    for (int i = 0; i < num_panels; ++i) {
        win = newwin(height, width, 0, 0);
        panels[i] = new_panel(win);
    }

    return panels;
}

/**
 * @brief Free memory allocated for the UI panels.
 *
 * @param panels Pointer to an array of panels.
 * @param num_panels The number of panels to free.
 */
void destroy_panels(PANEL **panels, int num_panels)
{
    for (int i = 0; i < num_panels; ++i) {
        delwin(panel_window(panels[i]));
        del_panel(panels[i]);
    }
    free(panels);
}

/**
 * @brief Creates and initializes the program UI.
 */
struct ui *ui_new(struct mpdwrapper *mpd)
{
    struct ui *ui = malloc(sizeof(*ui));
    if (!ui)
        return NULL;

    ui_initialize(ui, mpd);

    return ui;
}

void ui_initialize(struct ui *ui, struct mpdwrapper *mpd)
{
    getmaxyx(stdscr, ui->maxy, ui->maxx);

    ui->panels = create_panels(NUM_PANELS, ui->maxy - 2, ui->maxx);
    ui->visible_panel = QUEUE;
    top_panel(ui->panels[ui->visible_panel]);

    ui->queue = playlist_init(panel_window(ui->panels[QUEUE]));
    ui->statusbar = statusbar_new();
    ui->library = screen_library_new(ui->maxy - 2, ui->maxx);

    screen_library_populate_artists(ui->library, mpd);

    /* TODO: remove this once the playlist view gets refactored. */
    playlist_populate(ui->queue, mpdwrapper_get_queue(mpd));
}

void ui_free(struct ui *ui)
{
    playlist_free(ui->queue);
    statusbar_free(ui->statusbar);
    screen_library_free(ui->library);
    free(ui);
}

void ui_draw(struct ui *ui, struct mpdwrapper *mpd)
{
    statusbar_draw(ui->statusbar, mpd);

    WINDOW *win = panel_window(ui->panels[ui->visible_panel]);
    int is_playing;
    int is_paused;
    int current_song_id;

    if (mpdwrapper_queue_changed(mpd)) {
        playlist_clear(ui->queue);
        playlist_populate(ui->queue, mpdwrapper_get_queue(mpd));
    }

    switch (ui->visible_panel) {
        case HELP:
            draw_help_screen(win);
            break;
        case QUEUE:
            is_playing = mpdwrapper_is_playing(mpd);
            is_paused = mpdwrapper_is_paused(mpd);
            current_song_id = (is_playing || is_paused) ? mpdwrapper_get_current_song_id(mpd) : 0;
            playlist_draw(ui->queue, current_song_id);
            break;
        case LIBRARY:
            list_view_draw(ui->library->visible_view);
            break;
        default:
            break;
    }

    update_panels();

    doupdate();
}

void ui_set_visible_panel(struct ui *ui, enum ui_panel panel)
{
    ui->visible_panel = panel;
    top_panel(ui->panels[panel]);
}
