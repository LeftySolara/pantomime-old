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
#include "command.h"
#include "command_player.h"
#include "mpdclient.h"
#include "ui.h"

struct mpdclient *mpdclient;

int main(int argc, char **argv)
{
    mpdclient = mpdclient_init("localhost", 6600, 30000);
    struct ui *ui = ui_init(mpdclient);

    /* main loop */
    int ch;
    enum command cmd;
    halfdelay(TRUE);
    while (cmd != CMD_QUIT) {
        ch = getch();
        cmd = find_key_command(ch);

        if (cmd == CMD_QUIT)
            break;

        cmd_player(cmd, mpdclient);
        mpdclient_update(mpdclient);
        draw_ui(ui);
    }

    mpdclient_free(mpdclient);
    ui_free(ui);

    return 0;
}