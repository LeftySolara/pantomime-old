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
#include "labels.h"
#include "ui.h"

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

    ui->label_duration = NULL;
    ui->label_queue = NULL;
    ui->label_modes = NULL;

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
