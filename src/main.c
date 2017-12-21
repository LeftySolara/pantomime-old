/******************************************************************************
 * main.c : entry point for the program
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

#include <stdio.h>
#include "PantomimeConfig.h"
#include "mpdclient.h"
#include "ui.h"

int main(int argc, char **argv)
{
    ncurses_init();
    PANEL **panels = panels_init();
    struct mpdclient *mpd = mpdclient_connect("localhost", 6600, 30000);

    if (mpd->last_error == MPD_ERROR_SUCCESS)
        printw("Connected to MPD", panel_window(panels[QUEUE]));
    else
        printw("Unable to connect to MPD", panel_window(panels[QUEUE]));

    top_panel(panels[QUEUE]);
    update_panels();
    doupdate();
    getch();

    mpdclient_free(mpd);
    panels_free(panels);
    endwin();

    return 0;
}
