/*******************************************************************************
 * panel_help.h
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

#ifndef PANEL_HELP_H
#define PANEL_HELP_H

#include <ncurses.h>

#include "../command/command.h"

void draw_help_screen(WINDOW *win);
void draw_help_header(WINDOW *win, int begin_y, char *header_text);
void draw_command_info(WINDOW *win, int begin_y, enum command_type cmd);

#endif