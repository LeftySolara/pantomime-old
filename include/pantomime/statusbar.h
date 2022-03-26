/*******************************************************************************
 * statusbar.h
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

/**
 * @file statusbar.h
 */

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "pantomime/mpdwrapper.h"

struct statusbar;

struct statusbar *statusbar_new();
void statusbar_free(struct statusbar *statusbar);

void statusbar_draw(struct statusbar *statusbar, struct mpdwrapper *mpd);
void statusbar_set_notification(struct statusbar *statusbar, char *msg, int duration);

#endif /* STATUSBAR_H */
