/*******************************************************************************
 * ui.h
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

#include "ui.h"
#include "statusbar.h"

#include <locale.h>
#include <stdlib.h>

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

struct ui *ui_init()
{
    struct ui *ui = malloc(sizeof(*ui));
    getmaxyx(stdscr, ui->maxy, ui->maxx);

    ui->statusbar = newwin(2, ui->maxx, ui->maxy - 2, 0);
    ui->modes_label = malloc(sizeof(char) * 5);
    ui->song_label = malloc(sizeof(char) * 64);

    return ui;
}

void ui_free(struct ui *ui)
{
    delwin(ui->statusbar);
    free(ui->modes_label);
    free(ui->song_label);
    free(ui);
}

void ui_draw(struct ui *ui, struct mpdwrapper *mpd)
{
    draw_statusbar(ui->statusbar, mpd, ui->song_label, ui->modes_label);
    doupdate();
}