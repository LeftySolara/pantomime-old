/*******************************************************************************
 * pmenu.c
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
 * @file pmenu.h
 */

#include "pmenu.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Returns a pointer to a newly-created menu item.
 * 
 * Creates a new item to be used in a menu.
 * The item can be freed with menu_item_free().
 * 
 * @param text      The text to display in the item.
 * @param bold      Whether to print the item's text in bold.
 * @param highlight Whether the item should be highlighted in the menu.
 * 
 * @return A pointer to a menu item containing the specified text.
 */
struct menu_item *menu_item_init(char *text, int bold, int highlight)
{
    struct menu_item *item = malloc(sizeof(*item));

    item->text = malloc((strlen(text) + 1) * sizeof(char));
    sprintf(item->text, "%s", text);

    item->bold = bold;
    item->highlight = highlight;
    item->prev = NULL;
    item->next = NULL;

    return item;
}

/**
 * @brief Frees memory allocated by a menu item.
 */
void menu_item_free(struct menu_item *item)
{
    free(item->text);
    free(item);
    item = NULL;
}

/**
 * @brief Returns a pointer to a newly-created menu.
 * 
 * Creates a new menu with the specified header.
 * The menu can be freed with menu_free().
 * 
 * @param win       The window to draw menu contents on. 
 * @param header    The text to print above the list of items. 
 * 
 * @return          A pointer to a new menu.
 */
struct menu *menu_init(WINDOW *win, char *header)
{
    struct menu *menu = malloc(sizeof(*menu));

    menu->win = win;
    menu->header = malloc((strlen(header) + 1) * sizeof(char));
    
    menu->head = NULL;
    menu->tail = NULL;
    menu->selected = NULL;
    menu->top_visible = NULL;
    menu->bottem_visible = NULL;

    menu->length = 0;
    menu->selected_pos = -1;

    return menu;
}

/**
 * @brief Frees memory allocated by a menu.
 */
void menu_free(struct menu *menu)
{
    menu_clear(menu);
    free(menu);
}

/**
 * @brief Removes all items from a menu.
 * 
 * Removes all items from a menu and frees them.
 * This does not free the menu's memory or remove its header.
 */
void menu_clear(struct menu *menu)
{
    struct menu_item *current = menu->head;
    struct menu_item *next;

    while (current) {
        next = current->next;
        menu_item_free(current);
        current = next;
    }

    menu->head = NULL;
    menu->tail = NULL;
    menu->selected = NULL;
    menu->top_visible = NULL;
    menu->bottem_visible = NULL;
    menu->length = 0;
    menu->selected_pos = 0;
}