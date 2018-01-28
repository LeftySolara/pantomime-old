/******************************************************************************
 * command_queue.h : commands for interacting with the queue screen
 * ****************************************************************************
 * Copyright (C) 2018 Jalen Adams
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

#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "tracklist.h"
#include "command.h"

enum direction {UP, DOWN};

void move_cursor(struct tracklist *list, enum direction direction);
void cmd_queue(enum command cmd, struct tracklist *list);

#endif
