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

#include "list_view.h"
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

    this->lv_ops->lv_append = &list_view_append;
    this->lv_ops->lv_remove_selected = &list_view_remove_selected;
    this->lv_ops->lv_remove_at = &list_view_remove_at;
    this->lv_ops->lv_clear = &list_view_clear;
    this->lv_ops->lv_select = &list_view_select;
    this->lv_ops->lv_select_prev = &list_view_select_prev;
    this->lv_ops->lv_select_next = &list_view_select_next;
    this->lv_ops->lv_select_top_visible = &list_view_select_top_visible;
    this->lv_ops->lv_select_bottom_visible = &list_view_select_bottom_visible;
    this->lv_ops->lv_select_middle_visible = &list_view_select_middle_visible;
    this->lv_ops->lv_scroll_page_up = &list_view_scroll_page_up;
    this->lv_ops->lv_scroll_page_down = &list_view_scroll_page_down;
    this->lv_ops->lv_draw = &list_view_draw;
}

void list_view_free(struct list_view *this)
{
     list_view_clear(this);
     free(this);
}

void list_view_append(struct list_view *this, char *text)
{
    if (!this || !text)
        return;

    struct list_view_item *item = list_view_item_new(text);

    if (!this->head) {
        this->head = item;
        this->selected = item;
        this->top_visible = item;
        this->bottom_visible = item;
        this->idx_selected = 0;
    }
    else {
        struct list_view_item *tmp = this->tail;
        tmp->next = item;
        item->prev = tmp;
    }

    this->tail = item;
    this->item_count++;
}

void list_view_remove_selected(struct list_view *this) {}
void list_view_remove_at(struct list_view *this, int index) {}

/**
 * @brief Removes all items from a list view.
 * 
 * @param this The list view to clear.
 */
void list_view_clear(struct list_view *this)
{
    struct list_view_item *current = this->head;
    struct list_view_item *next;

    while (current) {
        next = current->next;
        list_view_item_free(current);
        current = next;
    }

    this->head = NULL;
    this->tail = NULL;
    this->selected = NULL;
    this->top_visible = NULL;
    this->bottom_visible = NULL;
    this->item_count = 0;
    this->idx_selected = -1;
}

void list_view_select(struct list_view *this, int index) {}
void list_view_select_prev(struct list_view *this) {}
void list_view_select_next(struct list_view *this) {}
void list_view_select_top_visible(struct list_view *this) {}
void list_view_select_bottom_visible(struct list_view *this) {}
void list_view_select_middle_visible(struct list_view *this) {}

void list_view_scroll_page_up(struct list_view *this) {}
void list_view_scroll_page_down(struct list_view *this) {}

void list_view_draw(struct list_view *this) {}
