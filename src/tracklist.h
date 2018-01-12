/******************************************************************************
 * tracklist.h : interface for displaying track lists
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

#ifndef TRACKLIST_H
#define TRACKLIST_H

#include <stdbool.h>
#include "mpdclient.h"

struct tracknode {
    struct tracknode *next;
    struct tracknode *prev;

    struct mpd_song *song;
    bool selected;
    bool playing;
};

struct tracklist {
    struct tracknode *head;
    struct tracknode *selected;
};

struct tracknode *tracknode_init();
struct tracklist *tracklist_init();

void tracknode_free(struct tracknode *node);
void tracklist_free(struct tracklist *list);

#endif
