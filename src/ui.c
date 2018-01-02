/******************************************************************************
 * ui.c : user-facing elements of the program
 * ****************************************************************************
 * Copyright (C) 2017 Jalen Adams
 *
 * Authors: Jalen Adams <leftysolara@gmail.com>
 *
 * This file is part of Pantomime.
 *
 * Pantomime is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pantomime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pantomime.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "mpdclient.h"
#include "labels.h"
#include "ui.h"

struct ui *ui_init()
{
    ncurses_init();
    struct ui *ui = malloc(sizeof(*ui));

    ui->label_duration = NULL;
    ui->label_queue = NULL;
    ui->label_modes = NULL;

    ui->panels = panels_init();
    top_panel(ui->panels[QUEUE]);

    ui->statusbar = newwin(2, COLS, LINES - 2, 0);

    ui->label_duration = create_label_duration(ui->label_duration);
    ui->label_queue = create_label_queue(ui->label_queue);
    ui->label_modes = create_label_modes(ui->label_modes);

    return ui;
}

void ui_free(struct ui *ui)
{
    panels_free(ui->panels);
    delwin(ui->statusbar);

    if (ui->label_duration)
        free(ui->label_duration);
    if (ui->label_queue)
        free(ui->label_queue);

    ui->statusbar = NULL;
    ui->panels = NULL;
    ui->label_queue = NULL;
    ui->label_duration = NULL;
    ui->label_modes = NULL;

    free(ui);
    endwin();
}

PANEL **panels_init()
{
    PANEL **panels = calloc(NUM_PANELS, sizeof(PANEL *));
    WINDOW *win;

    for (int i = 0; i < NUM_PANELS; ++i) {
        win = newwin(LINES - 5, COLS, 3, 1);
        panels[i] = new_panel(win);
    }

    return panels;
}

void panels_free(PANEL **panels)
{
    for (int i = 0; i < NUM_PANELS; ++i) {
        delwin(panel_window(panels[i]));
        del_panel(panels[i]);
    }
    free(panels);
}

void ncurses_init()
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


void draw_statusbar(struct ui *ui)
{
    enum mpd_state state = mpd_status_get_state(mpdclient->status);
    if (state == MPD_STATE_UNKNOWN)
        return;

    double song_length = mpd_song_get_duration(mpdclient->current_song);
    double time_elapsed = mpd_status_get_elapsed_time(mpdclient->status);
    double width = getmaxx(ui->statusbar);

    double secs_per_tick = song_length / width;     /* number of seconds before the bar progresses */
    double tick_size = (width / song_length) + 1;   /* number of characters to print per tick */
    double ticks_elapsed = time_elapsed / tick_size;

    wclear(ui->statusbar);

    /* draw the progress bar */
    if (state != MPD_STATE_STOP) {
        whline(ui->statusbar, '-', (tick_size * ticks_elapsed) / secs_per_tick);
        mvwaddch(ui->statusbar, 0, (tick_size * ticks_elapsed) / secs_per_tick, '>');
    }

    /* draw bottom labels */
    mvwaddstr(ui->statusbar, 1, 0, ui->label_queue);
    mvwaddstr(ui->statusbar, 1, COLS - strlen(ui->label_modes),
              ui->label_modes);

    wnoutrefresh(ui->statusbar);

}

void draw_ui(struct ui *ui)
{
    ui->label_duration = create_label_duration(ui->label_duration);
    ui->label_queue = create_label_queue(ui->label_queue);
    ui->label_modes = create_label_modes(ui->label_modes);

    draw_statusbar(ui);

    update_panels();
    doupdate();
}
