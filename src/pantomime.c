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

#include <argp.h>
#include <stdlib.h>

#include "command/command.h"
#include "command/command_global.h"
#include "command/command_library.h"
#include "command/command_player.h"
#include "command/command_queue.h"
#include "pantomime/mpdwrapper.h"
#include "pantomime/ui.h"

/* Argument Parsing */
const char *argp_program_version = "pantomime 0.1";
const char *argp_program_bug_address = "<julianne@julianneadams.info>";
static char doc[] = "Pantomime -- An NCURSES MPD client";

static struct argp_option options[] = {
    {"host", 'h', "HOST", 0, "The IP address or socket path of the MPD host"},
    {"port", 'p', "PORT", 0, "The port of the MPD host"},
    {"timeout", 't', "TIMEOUT", 0, "The timeout in milliseconds"},
    {0}};

/* Used by main() to communicate with parse_opt. */
struct arguments {
    char *args[0];
    char *host;
    int port;
    int timeout;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse */
    struct arguments *arguments = state->input;

    switch (key) {
        case 'h':
            arguments->host = arg;
            break;
        case 'p':
            arguments->port = atoi(arg);
            break;
        case 't':
            arguments->timeout = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 0) {
                /* Too many arguments. */
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, 0, doc};

int main(int argc, char **argv)
{
    /* Default arguments. */
    struct arguments arguments;
    arguments.host = "localhost";
    arguments.port = 6600;
    arguments.timeout = 30000;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    struct mpdwrapper *mpd = mpdwrapper_new(arguments.host, arguments.port, arguments.timeout);

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
