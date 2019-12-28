/*******************************************************************************
 * command_global.c
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

/**
 * @file command_global.h
 */

#include "command_global.h"
#include "../ui/statusbar.h"

/**
 * @brief Checks the input command and executes the related functions.
 */

void update_mpd_database(struct mpdwrapper *mpd, struct ui *ui)
{
    set_notification(ui->statusbar, "Starting database update...", 3);
    
    int rc = mpdwrapper_update_db(mpd);
    char *msg = (rc > 0) ? "Music database updated successfully" : "Error updating music database";

    set_notification(ui->statusbar, msg, 3);
}

void cmd_global(enum command_type cmd, struct mpdwrapper *mpd, struct ui *ui)
{
    switch (cmd) {
    case CMD_PANEL_HELP:
        set_visible_panel(ui, HELP);
        break;
    case CMD_PANEL_QUEUE:
        set_visible_panel(ui, QUEUE);
        break;
    case CMD_PANEL_LIBRARY:
        set_visible_panel(ui, LIBRARY);
        break;
    case CMD_DB_UPDATE:
        update_mpd_database(mpd, ui);
        break;
    default:
        break;
    }
}
