/*******************************************************************************
 * pmenu.h
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

/* ncurses does already provide a menu library, but it relies on having a fixed
 * number of items at all times. Since users will be able to modify what's
 * displayed (like on the queue screen, for example), a linked list is better
 * suited for this than an array. Thus, we're defining our own implementation.
 */

#ifndef PMENU_H
#define PMENU_H

#include <ncurses.h>

struct pmenu_item {
    char *text;     /**< The text to display. */
    int bold;       /**< Whether to print the text in bold. */
    int highlight;  /**< Whether to highlight this item after printing. */

    struct pmenu_item *prev;
    struct pmenu_item *next;
};

struct pmenu {
    WINDOW *win;

    struct pmenu_item *head;
    struct pmenu_item *tail;
    struct pmenu_item *selected;

    struct pmenu_item *top_visible;      /**< First item visible on the screen. */
    struct pmenu_item *bottom_visible;   /**< Last item visible on the screen. */

    char *header;               /**< The text to display above the list of items. */
    unsigned int length;        /**< The number of items in the menu. */
    unsigned int selected_pos;  /**< The index of the currently selected item. */
    unsigned int max_visible;   /**< The maximum number of items that can be displayed with the current window size. */
};

struct pmenu_item *pmenu_item_init(char *text, int bold, int highlight);
void pmenu_item_free(struct pmenu_item *item);

struct pmenu *pmenu_init(WINDOW *win, char *header);
void pmenu_free(struct pmenu *menu);

void pmenu_append(struct pmenu *menu, char *item_text, int bold, int highlight);

void pmenu_clear(struct pmenu *menu);

void pmenu_set_selected(struct pmenu *menu, unsigned int pos);
void pmenu_select_prev(struct pmenu *menu);
void pmenu_select_next(struct pmenu *menu);
void pmenu_select_top_visible(struct pmenu *menu);
void pmenu_select_bottom_visible(struct pmenu *menu);
void pmenu_select_middle_visible(struct pmenu *menu);

void pmenu_find_bottom(struct pmenu *menu);

void pmenu_item_draw(struct pmenu_item *item, WINDOW *win, unsigned y);
void pmenu_draw(struct pmenu *menu);

#endif /* PMENU_H */