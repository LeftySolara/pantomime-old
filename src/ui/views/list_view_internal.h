/*******************************************************************************
 * list_view_internal.h
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

#include "list_view.h"
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
};


struct list_view_item *list_view_item_new(char *text);
void list_view_item_initialize(struct list_view_item *this, char *text);
void list_view_item_free(struct list_view_item *this);

void list_view_initialize(struct list_view *this, int height, int width);

#endif /* LIST_VIEW_INTERNAL_H */