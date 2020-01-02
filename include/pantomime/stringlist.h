/*******************************************************************************
 * stringlist.h
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

#ifndef STRINGLIST_H
#define STRINGLIST_H

struct stringlist_item {
    char *str;
    struct stringlist_item *next;
    struct stringlist_item *prev;
};

struct stringlist {
    struct stringlist_item *head;
    struct stringlist_item *tail;
    int item_count;
};

struct stringlist_item *stringlist_item_new(char *str);
void stringlist_item_free(struct stringlist_item *item);

struct stringlist *stringlist_new();
void stringlist_free(struct stringlist *list);

void stringlist_append(struct stringlist *list, char *str);
void stringlist_remove(struct stringlist *list, int pos);
void stringlist_clear(struct stringlist *list);

#endif /* STRINGLIST_H */
