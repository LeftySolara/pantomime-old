/*******************************************************************************
 * args.h
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

#ifndef ARGS_H
#define ARGS_H

#include <argp.h>

/* Used by main() to communicate with parse_opt. */
struct args {
    char *arg1;
    char **strings;
    char *host;
    int port;
    int timeout;
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

struct args parse_args(int argc, char **argv);

#endif /* ARGS_H */
