/******************************************************************************
 * labels.h : functions for creating human-readable strings for MPD data
 * ****************************************************************************
 * Copyright (C) 2017 Jalen Adams
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

#ifndef LABELS_H
#define LABELS_H

#include <mpd/client.h>

char *create_label_song(char *buffer, struct mpd_song *song);
char *create_label_duration(char *buffer);
char *create_label_queue(char *buffer);
char *create_label_modes(char *buffer);
char *create_label_volume(char *buffer);

#endif
