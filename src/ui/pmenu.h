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

struct menu_item {
    char *text;     /**< The text to display. */
    int bold;       /**< Whether to print the text in bold. */
    int highlight;  /**< Whether to highlight this item after printing. */

    struct menu_item *prev;
    struct menu_item *next;
};

struct menu {
    WINDOW *win;

    struct menu_item *head;
    struct menu_item *tail;
    struct menu_item *selected;

    struct menu_item *top_visible;      /**< First item visible on the screen. */
    struct menu_item *bottem_visible;   /**< Last item visible on the screen. */

    char *header;               /**< The text to display above the list of items. */
    unsigned int length;        /**< The number of items in the menu. */
    unsigned int selected_pos;  /**< The index of the currently selected item. */
    unsigned int max_visible;   /**< The maximum number of items that can be displayed with the current window size. */
};

struct menu_item *menu_item_init(char *text, int bold, int highlight);
void menu_item_free(struct menu_item *item);

struct menu *menu_init(WINDOW *win, char *header);
void menu_free(struct menu *menu);

void menu_clear(struct menu *menu);

void menu_item_draw(struct menu_item *item, WINDOW *win, unsigned y);
void menu_draw(struct menu *menu);

#endif /* PMENU_H */