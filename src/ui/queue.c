/*******************************************************************************
 * queue.h
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

#include "queue.h"

#include <stdlib.h>

char *create_queue_header(WINDOW *win)
{
    int maxx = getmaxx(win);
    char *header = malloc(maxx * sizeof(char));

    /* The width of each column. There are four columns total but only three
     * are of variable size; the "Length" column is always 8 characters wide. */
    int field_width = (maxx - 8) / 3;

    sprintf(header, "%-*s%-*s%-*s%-s\n", field_width, "Artist", field_width, "Title",
            field_width, "Album", "Length");

    return header;
}