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
#include "config.h"

const char *argp_program_version = PANTOMIME_VERSION_NAMED;
const char *argp_program_bug_address = PANTOMIME_BUG_ADDRESS;

int main(int argc, char **argv)
{
    struct args args = parse_args(argc, argv);
    printf("%s %d %d\n", args.host, args.port, args.timeout);

    return 0;
}
