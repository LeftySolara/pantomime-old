/*******************************************************************************
 * command_queue.h
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

#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "command.h"
#include "../ui/panel_queue.h"

enum direction {UP, DOWN};

void move_cursor(struct queue_menu_list *list, enum direction direction);
void cmd_queue(enum command_type cmd, struct queue_menu_list *list);


#endif /* COMMAND_QUEUE_H */