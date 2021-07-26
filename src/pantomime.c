/*******************************************************************************
 * pantomime.c
 *******************************************************************************
 * Copyright (C) 2019  Jalen Adams
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "command/command.h"
#include "command/command_global.h"
#include "command/command_player.h"
#include "command/command_queue.h"
#include "command/command_library.h"
#include "pantomime/mpdwrapper.h"
#include "pantomime/ui.h"

int main()
{
    /* TODO: Allow user to specify connection settings as command line args
     *       or from a config file.
     */
    struct mpdwrapper *mpd = mpdwrapper_new("localhost", 6600, 30000);

    start_curses();
    halfdelay(TRUE);

    struct ui *ui = ui_new(mpd);
    ui_draw(ui, mpd);

    int ch;
    enum command_type cmd;

    while (cmd != CMD_QUIT) {
        mpdwrapper_refresh(mpd);

        ch = getch();
        cmd = find_key_command(ch);

        cmd_global(cmd, mpd, ui);
        cmd_player(cmd, mpd, ui->statusbar);

        switch (ui->visible_panel) {
        case HELP:
            break;
        case QUEUE:
            cmd_queue(cmd, mpd, ui);
            break;
        case LIBRARY:
            cmd_library(cmd, ui->library, ui->statusbar, mpd);
            break;
        default:
            break;
        }

        ui_draw(ui, mpd);
    }

    end_curses();
    ui_free(ui);
    mpdwrapper_free(mpd);

    return 0;
}
