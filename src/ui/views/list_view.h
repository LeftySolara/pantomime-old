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
 * @brief UI elements for displaying a navigable list.
 */

#ifndef LIST_VIEW_INTERNAL_H
#define LIST_VIEW_INTERNAL_H

#include "pantomime/list_view.h"
#include <ncurses.h>


struct list_view_item {
    char *text;     /**< The text to display for this item. */
    int bold;       /**< Whether to print the text in bold. */
    int highlight;  /**< Whether this item should be highlighted. */

    struct list_view_item *prev;    /**< The next item in the list. */
    struct list_view_item *next;    /**< The previous item in the list. */
};

struct list_view {
    WINDOW *win;    /**< The ncurses window to draw on. */

    struct list_view_item *head;        /**< The first item in the list. */
    struct list_view_item *tail;        /**< The last item in the list. */
    struct list_view_item *selected;    /**< The currently selected item. */

    struct list_view_item *top_visible;     /**< Topmost visible item in the list. */
    struct list_view_item *bottom_visible;  /**< Bottommost visible item in the list. */

    int item_count;     /**< The number of items in the list. */
    int idx_selected;   /**< The index of the currently selected item. */
    int max_visible;    /**< Max number of items that can be displayed with the current window dimensions. */

    const struct list_view_operations *lv_ops; /* Callbacks for list view. */
};

struct list_view_operations {
    void (*lv_append)(struct list_view *, char *);

    void (*lv_remove_selected)(struct list_view *);
    void (*lv_clear)(struct list_view *);

    void (*lv_select)(struct list_view *, int);
    void (*lv_select_prev)(struct list_view *);
    void (*lv_select_next)(struct list_view *);
    void (*lv_select_top_visible)(struct list_view *);
    void (*lv_select_bottom_visible)(struct list_view *);
    void (*lv_select_middle_visible)(struct list_view *);

    void (*lv_scroll_page_up)(struct list_view *);
    void (*lv_scroll_page_down)(struct list_view *);

    void (*lv_find_bottom)(struct list_view *);
    int (*lv_find_cursor_pos)(struct list_view *);

    void (*lv_draw)(struct list_view *);
};


struct list_view_item *list_view_item_new(char *text);
void list_view_item_initialize(struct list_view_item *this, char *text);
void list_view_item_free(struct list_view_item *this);
void list_view_item_draw(struct list_view_item *this, WINDOW *win, unsigned y);

void list_view_initialize(struct list_view *this, int height, int width);

void list_view_find_bottom(struct list_view *this);
int list_view_find_cursor_pos(struct list_view *this);

#endif /* LIST_VIEW_INTERNAL_H */
