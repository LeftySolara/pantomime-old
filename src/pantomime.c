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
#include "command/command_player.h"
#include "command/command_queue.h"
#include "mpdwrapper/mpdwrapper.h"
#include "ui/ui.h"

int main()
{
    start_curses();
    halfdelay(TRUE);

    /* TODO: Allow user to specify connection settings as command line args
     *       or from a config file.
     */
    struct mpdwrapper *mpd = mpdwrapper_init("localhost", 6600, 30000);

    struct ui *ui = ui_init(mpd);
    ui_draw(ui, mpd);

    int ch;
    enum command_type cmd;

    while (cmd != CMD_QUIT) {
        mpdwrapper_update(mpd);

        ch = getch();
        cmd = find_key_command(ch);

        cmd_player(cmd, mpd, ui->status_bar);

        switch (cmd) {
        case CMD_PANEL_HELP:
            set_visible_panel(ui, HELP);
            break;
        case CMD_PANEL_QUEUE:
            set_visible_panel(ui, QUEUE);
            break;
        default:
            break;
        }

        switch (ui->visible_panel) {
        case HELP:
            break;
        case QUEUE:
            cmd_queue(cmd, mpd, ui->queue_list);
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
