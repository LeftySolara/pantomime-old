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
        win = newwin(LINES - 2, COLS, 1, 1);
        panels[i] = new_panel(win);
    }

    return panels;
}

struct ui *ui_init(struct mpdclient *mpd)
{
    ncurses_init();
    struct ui *ui = malloc(sizeof(*ui));

    ui->panels = panels_init();
    top_panel(ui->panels[QUEUE]);

    ui->statusbar = statusbar_init(mpd);

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
    free(ui);
    endwin();
}

void ui_draw(struct ui *ui, struct mpdclient *mpd)
{
    statusbar_draw(ui->statusbar, mpd);
}
