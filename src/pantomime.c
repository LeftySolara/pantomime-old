/*******************************************************************************
 * pantomime.c
 *******************************************************************************
 * Copyright (C) 2017-2022  Julianne Adams
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

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "command/command.h"
#include "config.h"
#include "mpdwrapper/mpdwrapper.h"
#include "ui/ui.h"

const char *argp_program_version = PANTOMIME_VERSION_NAMED;
const char *argp_program_bug_address = PANTOMIME_BUG_ADDRESS;

/**
 * @brief Set the up mpd object.
 *
 * @param host The hostname, IP address, or UNIX socket of the MPD server.
 * @param port The TCP port to connect to, if using an IP address.
 * @param timeout The timeout in milliseconds.

 * @return A pointer to a new mpdwrapper object.
 */
struct mpdwrapper *setup_mpd(const char *host, int port, int timeout)
{
    struct mpdwrapper *mpd = mpdwrapper_init(host, port, timeout);
    if (!mpd) {
        fprintf(stderr, "Could not allocate memory for MPD connection.\n");
        exit(1);
    }
    if (mpd->last_error != MPD_ERROR_SUCCESS) {
        fprintf(stderr, "MPD error: %s\n", mpdwrapper_get_last_error_message(mpd));
        mpdwrapper_free(mpd);
        exit(mpd->last_error);
    }

    return mpd;
}

int main(int argc, char **argv)
{
    struct args args = parse_args(argc, argv);
    struct mpdwrapper *mpd = setup_mpd(args.host, args.port, args.timeout);

    start_ncurses();
    printw("Connected to mpd at %s\n", mpdwrapper_get_host(mpd));
    refresh();

    int ch;
    enum command_type cmd = CMD_NULL;

    while (cmd != CMD_QUIT) {
        ch = getch();
        cmd = find_key_command(ch);
    }

    stop_ncurses();
    mpdwrapper_free(mpd);

    exit(EXIT_SUCCESS);
}
