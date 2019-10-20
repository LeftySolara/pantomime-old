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
 * The item can be freed with pmenu_item_free().
 * 
 * @param text      The text to display in the item.
 * @param bold      Whether to print the item's text in bold.
 * @param highlight Whether the item should be highlighted in the menu.
 * 
 * @return A pointer to a menu item containing the specified text.
 */
struct pmenu_item *pmenu_item_init(char *text, int bold, int highlight)
{
    struct pmenu_item *item = malloc(sizeof(*item));

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
void pmenu_item_free(struct pmenu_item *item)
{
    free(item->text);
    free(item);
    item = NULL;
}

/**
 * @brief Returns a pointer to a newly-created menu.
 * 
 * Creates a new menu with the specified header.
 * The menu can be freed with pmenu_free().
 * 
 * @param win       The window to draw menu contents on. 
 * @param header    The text to print above the list of items. 
 * 
 * @return          A pointer to a new menu.
 */
struct pmenu *pmenu_init(WINDOW *win, char *header)
{
    struct pmenu *menu = malloc(sizeof(*menu));

    menu->win = win;

    menu->header = malloc((strlen(header) + 1) * sizeof(char));
    sprintf(menu->header, "%s\n", header);
    
    menu->head = NULL;
    menu->tail = NULL;
    menu->selected = NULL;
    menu->top_visible = NULL;
    menu->bottom_visible = NULL;

    menu->length = 0;
    menu->selected_pos = -1;
    menu->max_visible = getmaxy(win) - 2;

    return menu;
}

/**
 * @brief Frees memory allocated by a menu.
 */
void pmenu_free(struct pmenu *menu)
{
    pmenu_clear(menu);
    free(menu->header);
    free(menu);
}

/**
 * @brief Adds a new item to the end of a menu.
 */
void pmenu_append(struct pmenu *menu, char *item_text, int bold, int highlight)
{
    if (!menu)
        return;
    
    struct pmenu_item *new_item = pmenu_item_init(item_text, bold, highlight);

    if (!menu->head) {
        menu->head = new_item;
        menu->selected = new_item;
        menu->top_visible = new_item;
        menu->bottom_visible = new_item;
        menu->selected_pos = 0;
    }
    else {
        struct pmenu_item *tmp = menu->tail;
        tmp->next = new_item;
        new_item->prev = tmp;
    }

    menu->tail = new_item;
    menu->length++;
}

/**
 * @brief Removes all items from a menu.
 * 
 * Removes all items from a menu and frees them.
 * This does not free the menu's memory or remove its header.
 */
void pmenu_clear(struct pmenu *menu)
{
    struct pmenu_item *current = menu->head;
    struct pmenu_item *next;

    while (current) {
        next = current->next;
        pmenu_item_free(current);
        current = next;
    }

    menu->head = NULL;
    menu->tail = NULL;
    menu->selected = NULL;
    menu->top_visible = NULL;
    menu->bottom_visible = NULL;
    menu->length = 0;
    menu->selected_pos = 0;
}

/**
 * @brief Sets the item at the specified position as the currently selected item.
 */
void pmenu_set_selected(struct pmenu *menu, unsigned int pos)
{
    if (pos >= menu->length)
        return;

    struct pmenu_item *current = menu->head;
    for (int i = 0; i < pos; ++i)
        current = current->next;

    menu->selected = current;
    menu->selected_pos = pos;
    current->highlight = 1;

    current = current->next;
    while (current) {
        current->highlight = 0;
        current = current->next;
    }
}

/**
 * @brief Selects the previous item in the menu.
 */
void pmenu_select_prev(struct pmenu *menu)
{
    if (!menu || menu->selected == menu->head)
        return;

    struct pmenu_item *current = menu->selected;
    current->highlight = 0;
    if (current == menu->top_visible) {
        menu->top_visible = current->prev;
        pmenu_find_bottom(menu);
    }

    current = current->prev;
    current->highlight = 1;

    menu->selected = menu->selected->prev;
    menu->selected_pos--;
}

/**
 * @brief Selects the next item in the menu.
 */
void pmenu_select_next(struct pmenu *menu)
{
    if (!menu || menu->selected == menu->tail)
        return;

    struct pmenu_item *current = menu->selected;
    current->highlight = 0;
    if (current == menu->bottom_visible && current->next) {
        menu->top_visible = menu->top_visible->next;
        pmenu_find_bottom(menu);
    }

    current = current->next;
    current->highlight = 1;

    menu->selected = menu->selected->next;
    menu->selected_pos++;
}

/**
 * @brief Selects the first visible item in the menu.
 */
void pmenu_select_top_visible(struct pmenu *menu)
{
    if (!menu || menu->selected == menu->top_visible)
        return;
    
    struct pmenu_item *current = menu->selected;
    current->highlight = 0;

    while (current != menu->top_visible) {
        current = current->prev;
        menu->selected_pos--;
    }

    current->highlight = 1;
    menu->selected = current;
}

/**
 * @brief Selects the last visible item in the menu.
 */
void pmenu_select_bottom_visible(struct pmenu *menu)
{
    if (!menu || menu->selected == menu->bottom_visible)
        return;
    
    struct pmenu_item *current = menu->selected;
    current->highlight = 0;

    while (current != menu->bottom_visible) {
        current = current->next;
        menu->selected_pos++;
    }

    current->highlight = 1;
    menu->selected = current;
}

/**
 * @brief Selects the item in the middle of the visible portion of the menu.
 */
void pmenu_select_middle_visible(struct pmenu *menu)
{
    if (!menu || menu->length == 0)
        return;
    
    int midpoint = menu->max_visible / 2;

    pmenu_select_top_visible(menu);
    for (int i = 0; i < midpoint; ++i)
        pmenu_select_next(menu);
}

/**
 * @brief Scrolls up one page.
 */
void pmenu_scroll_page_up(struct pmenu *menu)
{
    if (!menu || menu->length == 0)
        return;
    
    for (int i = 0; i <= menu->max_visible; ++i)
        pmenu_select_prev(menu);
}

/**
 * @brief Scrolls one page down.
 */
void pmenu_scroll_page_down(struct pmenu *menu)
{
    if (!menu || menu->length == 0)
        return;

    pmenu_select_bottom_visible(menu);
    if (!menu->selected->next)
        return;

    pmenu_select_next(menu);
    menu->top_visible = menu->selected;
    pmenu_find_bottom(menu);
}

/**
 * @brief Calculates which menu item is the bottommost visible.
 */
void pmenu_find_bottom(struct pmenu *menu)
{
    int rows = getmaxy(menu->win);
    struct pmenu_item *current = menu->top_visible;

    /* Only iterate up to rows-2 to account for the header and
     * off-by-one errors. */
    for (int i = 0; i < rows - 2; ++i) {
        if (!current->next)
            break;
        current = current->next;
    }
    menu->bottom_visible = current;
}

/**
 * @brief Draws a menu item on the screen.
 * 
 * @param item  The item to draw.
 * @param win   The window to draw the item on.
 * @param y     The y-position for drawing the item.
 */
void pmenu_item_draw(struct pmenu_item *item, WINDOW *win, unsigned y)
{
    if (item->bold)
        wattr_on(win, A_BOLD, 0);
    if (item->highlight)
        wattr_on(win, A_STANDOUT, 0);

    mvwprintw(win, y, 0, "%s\n", item->text);

    if (item->highlight)
        mvwchgat(win, y, 0, -1, A_STANDOUT, 0, NULL);
    wattr_off(win, A_BOLD, 0);
    wattr_off(win, A_STANDOUT, 0);
}

/**
 * @brief Draws a menu on the screen.
 */
void pmenu_draw(struct pmenu *menu)
{
    werase(menu->win);

    wattr_on(menu->win, A_BOLD, NULL);
    mvwprintw(menu->win, 0, 0, menu->header);
    wattr_off(menu->win, A_BOLD, NULL);

    if (menu->length <= 0)
        return;
    if (!menu->selected) {
        menu->selected = menu->head;
        menu->selected_pos = 0;
    }

    int y = 1;
    struct pmenu_item *current = menu->top_visible;
    while (current != menu->bottom_visible->next) {
        pmenu_item_draw(current, menu->win, y++);
        current = current->next;
    }

    wnoutrefresh(menu->win);
}