/*******************************************************************************
 * command_library.h
 *******************************************************************************
 * Copyright (C) 2019-2022  Julianne Adams
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

#ifndef COMMAND_LIBRARY_H
#define COMMAND_LIBRARY_H

#include "../ui/screens/screen_library.h"
#include "command.h"
#include "pantomime/statusbar.h"

void cmd_add_artist(struct screen_library *screen, struct statusbar *statusbar,
                    struct mpdwrapper *mpd);
void cmd_add_album(struct screen_library *screen, struct statusbar *statusbar,
                   struct mpdwrapper *mpd);
void cmd_add_song(struct screen_library *screen, struct statusbar *statusbar,
                  struct mpdwrapper *mpd);
void cmd_add_to_queue(struct screen_library *screen, struct statusbar *statusbar,
                      struct mpdwrapper *mpd);

void cmd_library(enum command_type cmd, struct screen_library *screen, struct statusbar *statusbar,
                 struct mpdwrapper *mpd);

#endif /* COMMAND_LIBRARY_H */
