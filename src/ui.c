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
#include "ui.h"

extern struct mpdclient *mpd;

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

struct ui *ui_init()
{
    ncurses_init();
    struct ui *ui = malloc(sizeof(*ui));

    ui->panels = panels_init();
    top_panel(ui->panels[QUEUE]);

    ui->statusbar = statusbar_init();
    ui->label_duration = create_label_duration(ui->label_duration);
    ui->label_queue = create_label_queue(ui->label_queue);
    ui->label_modes = create_label_modes(ui->label_modes);

    return ui;
}

void panels_free(PANEL **panels)
{
    for (int i = 0; i < NUM_PANELS; ++i) {
        delwin(panel_window(panels[i]));
        del_panel(panels[i]);
    }
    free(panels);
}

void ui_free(struct ui *ui)
{
    panels_free(ui->panels);
    statusbar_free(ui->statusbar);
    if (ui->label_duration)
        free(ui->label_duration);
    if (ui->label_queue)
        free(ui->label_queue);

    free(ui);
    endwin();
}

void ui_draw(struct ui *ui)
{
    ui->label_duration = create_label_duration(ui->label_duration);
    ui->label_queue = create_label_queue(ui->label_queue);
    ui->label_modes = create_label_modes(ui->label_modes);

    statusbar_draw(ui->statusbar, ui);
    update_panels();
    doupdate();
}

/* Create a label for the progress of the currently playing song */
char *create_label_duration(char *buffer)
{
    if (mpdclient->state == MPD_STATE_UNKNOWN || mpdclient->state == MPD_STATE_STOP)
        return NULL;

    unsigned int total_time = mpd_status_get_total_time(mpdclient->status);
    unsigned int elapsed_time = mpd_status_get_elapsed_time(mpdclient->status);
    const size_t label_size = strlen("123:45 / 123:45") + 1;

    unsigned int total_minutes = total_time / 60;
    unsigned int total_seconds = total_time % 60;
    unsigned int elapsed_minutes = elapsed_time / 60;
    unsigned int elapsed_seconds = elapsed_time % 60;

    buffer = realloc(buffer, label_size);
    snprintf(buffer, label_size, "%d:%02d / %d:%02d",
             elapsed_minutes, elapsed_seconds, total_minutes, total_seconds);

    return buffer;

}

/* Create a label for the number of songs in the queue */
char *create_label_queue(char *buffer)
{
    /* account for max number of digits in an unsigned int and a null character */
    const size_t label_size = strlen("songs in queue") + 12;
    unsigned int queue_length = mpd_status_get_queue_length(mpdclient->status);

    buffer = realloc(buffer, label_size);
    snprintf(buffer, label_size, "%u %s", queue_length, "songs in queue");

    return buffer;
}

char *create_label_modes(char *buffer)
{
    const int mode_count = 4; /* mpd has four modes built in */
    buffer = realloc(buffer, mode_count + 1);
    memset(buffer, '-', mode_count);

    if (mpd_status_get_repeat(mpdclient->status))
        buffer[0] = 'r';
    if (mpd_status_get_random(mpdclient->status))
        buffer[1] = 'z';
    if (mpd_status_get_single(mpdclient->status))
        buffer[2] = 's';
    if (mpd_status_get_consume(mpdclient->status))
        buffer[3] = 'c';

    return buffer;
}
