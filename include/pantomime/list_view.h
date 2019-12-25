/*******************************************************************************
 * list_view.h
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

/**
 * @file list_view.h
 */

#ifndef LIST_VIEW_H
#define LIST_VIEW_H

struct list_view;

struct list_view *list_view_new(int height, int width);
void list_view_free(struct list_view *this);

void list_view_append(struct list_view *this, char *text);

void list_view_remove_selected(struct list_view *this);
void list_view_clear(struct list_view *this);

void list_view_select(struct list_view *this, int index);
void list_view_select_prev(struct list_view *this);
void list_view_select_next(struct list_view *this);
void list_view_select_top_visible(struct list_view *this);
void list_view_select_bottom_visible(struct list_view *this);
void list_view_select_middle_visible(struct list_view *this);

void list_view_scroll_page_up(struct list_view *this);
void list_view_scroll_page_down(struct list_view *this);

void list_view_draw(struct list_view *this);

#endif /* LIST_VIEW_H */