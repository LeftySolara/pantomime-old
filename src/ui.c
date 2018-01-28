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
#include "screen_queue.h"
#include "ui.h"

struct ui *ui_init()
{
    ncurses_init();

    struct ui *ui = malloc(sizeof(*ui));
    getmaxyx(stdscr, ui->maxy, ui->maxx);

    ui->panels = panels_init(ui);
    ui->visible_panel = QUEUE;
    top_panel(ui->panels[ui->visible_panel]);

    ui->headerbar = newwin(3, ui->maxx, 0, 0);
    ui->statusbar = newwin(2, ui->maxx, ui->maxy - 2, 0);

    ui->labels = calloc(NUM_LABELS - 1, sizeof(char *));
    ui->labels[PROGRESS] = create_label_progress(ui->labels[PROGRESS]);
    ui->labels[QUEUE_LEN] = create_label_queue(ui->labels[QUEUE_LEN]);
    ui->labels[MODES] = create_label_modes(ui->labels[MODES]);
    ui->labels[VOLUME] = create_label_volume(ui->labels[VOLUME]);
    ui->labels[CURRENT_SONG] = create_label_song(
        ui->labels[CURRENT_SONG], mpdclient->current_song);

    return ui;
}

void ui_free(struct ui *ui)
{
    panels_free(ui->panels);
    delwin(ui->headerbar);
    delwin(ui->statusbar);

    ui->headerbar = NULL;
    ui->statusbar = NULL;
    ui->panels = NULL;

    for (int i = 0; i < NUM_LABELS; ++i)
        free(ui->labels[i]);
    free(ui->labels);

    free(ui);
    endwin();
}

PANEL **panels_init(struct ui *ui)
{
    PANEL **panels = calloc(NUM_PANELS, sizeof(PANEL *));
    WINDOW *win;

    for (int i = 0; i < NUM_PANELS; ++i) {
        win = newwin(ui->maxy - 5, ui->maxx, 3, 0);
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

void draw_headerbar(struct ui *ui)
{
    wclear(ui->headerbar);
    if (ui->labels[PROGRESS])
        mvwaddstr(ui->headerbar, 1, 0, ui->labels[PROGRESS]);
    else
        mvwaddstr(ui->headerbar, 1, 0, "0:00 / 0:00");

    if (ui->labels[CURRENT_SONG]) {
        int startx = (ui->maxx / 2) - (strlen(ui->labels[CURRENT_SONG]) / 2);
        mvwaddstr(ui->headerbar, 1, startx, ui->labels[CURRENT_SONG]);
    }
    mvwaddstr(ui->headerbar, 1, ui->maxx - strlen(ui->labels[VOLUME]), ui->labels[VOLUME]);

    wnoutrefresh(ui->headerbar);
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
        wattr_on(ui->statusbar, A_BOLD, NULL);
        whline(ui->statusbar, '-', (tick_size * ticks_elapsed) / secs_per_tick);
        mvwaddch(ui->statusbar, 0, (tick_size * ticks_elapsed) / secs_per_tick, '>');
        wattr_off(ui->statusbar, A_BOLD, NULL);
    }

    /* draw bottom labels */
    mvwaddstr(ui->statusbar, 1, 0, ui->labels[QUEUE_LEN]);
    mvwaddstr(ui->statusbar, 1, ui->maxx - strlen(ui->labels[MODES]),
              ui->labels[MODES]);

    wnoutrefresh(ui->statusbar);
}

void draw_ui(struct ui *ui)
{
    ui->labels[PROGRESS] = create_label_progress(ui->labels[PROGRESS]);
    ui->labels[QUEUE_LEN] = create_label_queue(ui->labels[QUEUE_LEN]);
    ui->labels[MODES] = create_label_modes(ui->labels[MODES]);
    ui->labels[VOLUME] = create_label_volume(ui->labels[VOLUME]);
    ui->labels[CURRENT_SONG] = create_label_song(
        ui->labels[CURRENT_SONG], mpdclient->current_song);

    draw_headerbar(ui);
    draw_statusbar(ui);

    switch(ui->visible_panel) {
    case QUEUE:
        draw_queue(mpdclient->queue, mpdclient->current_song,
                   panel_window(ui->panels[ui->visible_panel]));
        break;
    default:
        break;
    }

    update_panels();
    doupdate();
}
