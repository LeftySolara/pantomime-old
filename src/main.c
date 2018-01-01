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
    struct mpdclient *mpd = mpdclient_connect("localhost", 6600, 30000);
    struct ui *ui = ui_init(mpd);

    /* main loop */
    char ch;
    halfdelay(TRUE);
    while ((ch = getch() != 'q')) {
        mpd->status = mpd_run_status(mpd->connection);
        ui_draw(ui, mpd);
    }

    mpdclient_free(mpd);
    ui_free(ui);

    return 0;
}
