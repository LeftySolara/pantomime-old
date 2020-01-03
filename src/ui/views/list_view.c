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

/**
 * @file list_view.h
 */

#include "list_view.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Creates an item for use in a list view.
 * The caller is responsible for freeing any dynamically
 * allocated pointer that is passed here.
 */
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
    const size_t text_len = strlen(text) + 1;
    this->text = realloc(this->text, text_len * sizeof(char));
    snprintf(this->text, text_len, "%s", text);

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

void list_view_item_draw(struct list_view_item *this, WINDOW *win, unsigned y)
{
    if (this->bold)
        wattr_on(win, A_BOLD, 0);
    if (this->highlight)
        wattr_on(win, A_STANDOUT, 0);

    mvwprintw(win, y, 0, this->text);

    if (this->highlight)
        mvwchgat(win, y, 0, -1, A_STANDOUT, 0, NULL);
    wattr_off(win, A_BOLD, 0);
    wattr_off(win, A_STANDOUT, 0);
}


static const struct list_view_operations lv_ops = {
    .lv_append = list_view_append,
    .lv_remove_selected = list_view_remove_selected,
    .lv_clear = list_view_clear,
    .lv_select = list_view_select,
    .lv_select_prev = list_view_select_prev,
    .lv_select_next = list_view_select_next,
    .lv_select_top_visible = list_view_select_top_visible,
    .lv_select_bottom_visible = list_view_select_bottom_visible,
    .lv_select_middle_visible = list_view_select_middle_visible,
    .lv_scroll_page_up = list_view_scroll_page_up,
    .lv_scroll_page_down = list_view_scroll_page_down,
    .lv_find_bottom = list_view_find_bottom,
    .lv_find_cursor_pos = list_view_find_cursor_pos,
    .lv_draw = list_view_draw
};

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

    this->lv_ops = &lv_ops;
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

void list_view_remove_selected(struct list_view *this)
{
    if (!this || !this->head)
        return;

    if (this->selected == this->head) {
        if (!this->head->next)  /* Only one item in the list. */
            this->lv_ops->lv_clear(this);
        else {
            struct list_view_item *current = this->head;
            this->lv_ops->lv_select_next(this);
            this->top_visible = this->selected;
            this->lv_ops->lv_find_bottom(this);
            this->head->prev = NULL;

            list_view_item_free(current);
            this->head = this->selected;
        }
        this->item_count--;
        this->idx_selected = 0;
    }
    else {
        struct list_view_item *current = this->selected;

        if (current->next)
            this->lv_ops->lv_select_next(this);
        else
            this->lv_ops->lv_select_prev(this);

        if (current == this->top_visible)
            this->top_visible = this->selected;
        if (current == this->tail) {
            this->tail = this->tail->prev;
            this->idx_selected++;
        }

        if (current->prev)
            current->prev->next = current->next;
        if (current->next)
            current->next->prev = current->prev;

        list_view_item_free(current);
        this->item_count--;
        this->idx_selected--;
        this->lv_ops->lv_find_bottom(this);
    }
}

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

/**
 * @brief Sets the item at the specified index as the currently selected item.
 */
void list_view_select(struct list_view *this, int index)
{
    if (index < 0 || index >= this->item_count)
        return;

    struct list_view_item *current = this->head;
    for (int i = 0; i < index; ++i)
        current = current->next;

    this->selected = current;
    this->idx_selected = index;
    current->highlight = 1;

    current = current->next;
    while (current) {
        current->highlight = 0;
        current = current->next;
    }
}

/**
 * @brief Selects the previous item in the list.
 */
void list_view_select_prev(struct list_view *this)
{
    if (!this || this->selected == this->head)
        return;

    struct list_view_item *current = this->selected;
    current->highlight = 0;
    if (current == this->top_visible) {
        this->top_visible = current->prev;
        this->lv_ops->lv_find_bottom(this);
    }
    current = current->prev;
    current->highlight = 1;

    this->selected = this->selected->prev;
    this->idx_selected--;
}

/**
 * @brief Selects the next item in the list.
 */
void list_view_select_next(struct list_view *this)
{
    if (!this || this->selected == this->tail)
        return;

    struct list_view_item *current = this->selected;
    current->highlight = 0;
    if (current == this->bottom_visible && current->next) {
        this->top_visible = this->top_visible->next;
        this->lv_ops->lv_find_bottom(this);
    }
    
    current = current->next;
    current->highlight = 1;

    this->selected = this->selected->next;
    this->idx_selected++;
}

/**
 * @brief Sets the first visible item in the list as the selected item.
 */
void list_view_select_top_visible(struct list_view *this)
{
    if (!this || this->selected == this->top_visible)
        return;

    struct list_view_item *current = this->selected;
    current->highlight = 0;

    while (current != this->top_visible) {
        current = current->prev;
        this->idx_selected--;
    }

    current->highlight = 1;
    this->selected = current;
}

/**
 * @brief Sets the last visible item in the list as the selected item.
 */
void list_view_select_bottom_visible(struct list_view *this)
{
    if (!this || this->selected == this->bottom_visible)
        return;

    struct list_view_item *current = this->selected;
    current->highlight = 0;

    while (current != this->bottom_visible) {
        current = current->next;
        this->idx_selected++;
    }

    current->highlight = 1;
    this->selected = current;
}

/**
 * @brief Sets the item in the middle of the window as the selected item.
 */
void list_view_select_middle_visible(struct list_view *this)
{
    if (!this || this->item_count == 0)
        return;

    int midpoint = this->item_count / 2;

    this->lv_ops->lv_select_top_visible(this);
    for (int i = 0; i < midpoint; ++i)
        this->lv_ops->lv_select_next(this);
}

/**
 * @brief Scrolls up one page in the list.
 */
void list_view_scroll_page_up(struct list_view *this)
{
    if (!this || this->item_count == 0)
        return;

    /* For restoring cursor position, if necessary. */
    int y_pos = this->lv_ops->lv_find_cursor_pos(this);

    this->lv_ops->lv_select_bottom_visible(this);
    if (!this->selected->next)
        return;

    this->lv_ops->lv_select_next(this);
    this->top_visible = this->selected;
    this->lv_ops->lv_find_bottom(this);

    /* Restore cursor position */
    this->lv_ops->lv_select_top_visible(this);
    for (int i = 0; i < y_pos; ++i)
        this->lv_ops->lv_select_next(this);
}

/**
 * @brief Scrolls down one page in the list.
 */
void list_view_scroll_page_down(struct list_view *this)
{
    if (!this || this->item_count == 0)
        return;

    /* For restoring cursor position, if necessary. */
    int y_pos = this->lv_ops->lv_find_cursor_pos(this);

    this->lv_ops->lv_select_bottom_visible(this);
    if (!this->selected->next)
        return;

    this->lv_ops->lv_select_next(this);
    this->top_visible = this->selected;
    this->lv_ops->lv_find_bottom(this);

    /* Restore cursor position. */
    this->lv_ops->lv_select_top_visible(this);
    for (int i = 0; i < y_pos; ++i)
        this->lv_ops->lv_select_next(this);
}

/* Draws the list view on its window. */
void list_view_draw(struct list_view *this)
{
    werase(this->win);

    if (this->item_count <= 0)
        return;
    if (!this->selected)
        this->lv_ops->lv_select_top_visible(this);

    /* Trying to draw the whole list at once and scrolling thorugh it
     * doesn't work because drawing past the bounds of an ncurses window
     * does nothing (this can kind of work with an ncurses pad, but that doesn't
     * allow for list selection, navigation, and manipulation in the way we want).
     * Because of this, it's more efficient to just worry about the items
     * that are currently visible and redraw manually when there's a change.
     * 
     * Since we know which item is displayed at the top and have the window dimensions,
     * we can figure out which item will be the last one visible and only draw
     * the ones in that range.
     */
    struct list_view_item *current = this->top_visible;
    this->lv_ops->lv_find_bottom(this);

    int y = 1;
    while (current != this->bottom_visible->next) {
        list_view_item_draw(current, this->win, y++);
        current = current->next;
    }

    wnoutrefresh(this->win);
}

/*
 * Calculates which list item is the bottommost visible.
 */
void list_view_find_bottom(struct list_view *this)
{
    if (!this)
        return;

    int rows = getmaxy(this->win);
    struct list_view_item *current = this->top_visible;

    for (int i = 0; i < rows - 2; ++i) {
        if (!current->next)
            break;
        current = current->next;
    }
    this->bottom_visible = current;
}

/*
 * Finds the y-position of the cursor in the window.
 */
int list_view_find_cursor_pos(struct list_view *this)
{
    if (!this->top_visible)
        return -1;

    struct list_view_item *current = this->top_visible;
    int y_pos = 0;

    while (current != this->selected) {
        ++y_pos;
        current = current->next;
    }

    return y_pos;
}

