/*******************************************************************************
 * ui.h
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

#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H

#include "pantomime/ui.h"
#include "pantomime/statusbar.h"
#include "views/list_view.h"
#include "playlist.h"

#include <ncurses.h>
#include <panel.h>

#define DEFAULT_NOTIFICATION_LENGTH 3

struct ui {
    PANEL **panels;
    enum ui_panel visible_panel; /* Only one panel should be visible at a time. */

    struct playlist *queue;
    struct statusbar *statusbar;
    struct list_view *library;

    int maxx;
    int maxy;
};

void start_curses();
void end_curses();

PANEL **create_panels(int num_panels, int width, int height);
void destroy_panels(PANEL **panels, int num_panels);

void ui_initialize(struct ui *ui, struct mpdwrapper *mpd);


#endif /* UI_INTERNAL)H */
