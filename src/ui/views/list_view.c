/*******************************************************************************
 * list_view.c
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

#include "list_view_internal.h"
#include <stdlib.h>


struct list_view_item *list_view_item_new(char *text)
{
    struct list_view_item *item = malloc(sizeof(*item));
    if (!item)
        return NULL;

    list_view_item_initialize(item, text);

    return item;
}

void list_view_item_initialize(struct list_view_item *this, char *text)
{
    this->text = text;
    this->bold = 0;
    this->highlight = 0;
    this->prev = NULL;
    this->next = NULL;
}

void list_view_item_free(struct list_view_item *this)
{
    if (!this)
        return;

    free(this->text);
    free(this);
}

struct list_view *list_view_new(int height, int width)
{
    struct list_view *view = malloc(sizeof(*view));
    if (!view)
        return NULL;

    list_view_initialize(view, height, width);

    return view;
}

void list_view_initialize(struct list_view *this, int height, int width)
{
    this->win = newwin(height, width, 0, 0);

    this->head = NULL;
    this->tail = NULL;
    this->selected = NULL;

    this->top_visible = NULL;
    this->bottom_visible = NULL;

    this->item_count = 0;
    this->idx_selected = -1;
    this->max_visible = getmaxy(this->win) - 1;
}

void list_view_free(struct list_view *this)
{
     /* list_view_clear(this);  Not implemented yet */
     free(this);
}