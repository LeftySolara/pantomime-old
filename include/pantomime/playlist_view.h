/*******************************************************************************
 * playlist_view.h
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

#ifndef PLAYLIST_VIEW_H
#define PLAYLIST_VIEW_H

struct playlist_view;

struct playlist_view *playlist_view_new(int height, int width);
void playlist_view_free(struct playlist_view *this);

#endif