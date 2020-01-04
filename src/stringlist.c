/*******************************************************************************
 * stringlist.c
 *******************************************************************************
 * Copyright (C) 2017-2020  Jalen Adams
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file stringlist.h
 */

#include "pantomime/stringlist.h"
#include <string.h>
#include <stdlib.h>


struct stringlist_item *stringlist_item_new(char *str)
{
    struct stringlist_item *item = malloc(sizeof(*item));
    if (!item)
        return NULL;

    item->str = str;
    item->prev = NULL;
    item->next = NULL;

    return item;
}

void stringlist_item_free(struct stringlist_item *item)
{
    free(item);
}

struct stringlist *stringlist_new()
{
    struct stringlist *list = malloc(sizeof(*list));
    if (!list)
        return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->item_count = 0;
    
    return list;
}

void stringlist_free(struct stringlist *list)
{
    stringlist_clear(list);
    free(list);
}

void stringlist_append(struct stringlist *list, char *str)
{
    if (!list || !str)
        return;

    struct stringlist_item *item = stringlist_item_new(str);

    if (!list->head) {
        list->head = item;
    }
    else {
        struct stringlist_item *tmp = list->tail;
        tmp->next = item;
        item->prev = tmp;
    }

    list->tail = item;
    list->item_count++;
}

void stringlist_remove(struct stringlist *list, int pos)
{
    if (!list || !list->head || pos >= list->item_count)
        return;

    if (pos == 0) {
        if (!list->head->next)
            stringlist_clear(list);
        else {
            struct stringlist_item *current = list->head;
            struct stringlist_item *new_head = list->head->next;

            current->prev = NULL;
            list->head = new_head;
            stringlist_item_free(current);
        }
        list->item_count--;
    }
    else {
        struct stringlist_item *current = list->head;
        
        for (int i = 0; i < pos; ++i)
            current = current->next;
        
        if (current == list->tail)
            list->tail = list->tail->prev;
        if (current->prev)
            current->prev->next = current->next;
        if (current->next)
            current->next->prev = current->prev;

        stringlist_item_free(current);
        list->item_count--;;
    }
}

void stringlist_clear(struct stringlist *list)
{
    struct stringlist_item *current = list->head;
    struct stringlist_item *next;

    while (current) {
        next = current->next;
        stringlist_item_free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->item_count = 0;
}
