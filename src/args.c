/*******************************************************************************
 * args.c
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

#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

/**
 * @brief Get the current program version.
 *
 * @return char* Pointer to the newly-allocated string containing the version number.
 *
 * This function fetches the current version number from the CMake build system.
 * The space for the output string is allocated as if malloc() were called.
 * To avoid memory leaks, the output string must be passed to free().
 */
char *get_program_version(void)
{
    size_t bufsz = strlen("Pantomime 0.0.0");
    char *version = malloc(sizeof(char) * bufsz);
    snprintf(version, bufsz + 1, "%s %s", PROJECT, PANTOMIME_VERSION);

    return version;
}

/**
 * @brief Parse a single option.
 */
error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct args *args = state->input;

    switch (key) {
        case 'h':
            args->host = arg;
            break;
        case 'p':
            args->port = atoi(arg);
            break;
        case 't':
            args->timeout = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num > 0) {
                /* Too many arguments. */
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/**
 * @brief Parse command-line arguments.
 */
struct args parse_args(int argc, char **argv)
{
    static char args_doc[] = "Pantomime -- An NCURSES MPD client";

    static struct argp_option opts[] = {
        {"host", 'h', "HOST", 0, "The hostname, IP address, or socket path of the MPD server", 0},
        {"port", 'p', "PORT", 0, "The TCP port to connect to on the MPD server", 0},
        {"timeout", 't', "TIMEOUT", 0, "The timeout in milliseconds", 0},
        {0}};

    static struct argp argp = {opts, parse_opt, NULL, args_doc, NULL, NULL, NULL};

    struct args args;
    argp_parse(&argp, argc, argv, 0, 0, &args);

    return args;
}
