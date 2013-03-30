// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>

#include "colors.h"
#include "yacursex.h"

using namespace YAPET::UI;

bool Colors::__initialized = false;

#ifndef _GIRLYCOLORS
color Colors::__colors[] = {
    // Normal
    {1, COLOR_WHITE, COLOR_BLACK, A_NORMAL},
    // Message box title
    {2, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // Message box
    {3, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Input widget no focus
    {4, COLOR_BLACK, COLOR_WHITE, A_REVERSE},
    // Input widget focus
    {5, COLOR_BLACK, COLOR_GREEN, A_REVERSE},
    // Input widget hidden (Only works with colors)
    {6, COLOR_YELLOW, COLOR_YELLOW, A_REVERSE},
    // Button no focus
    {7, COLOR_WHITE, COLOR_BLACK, A_NORMAL},
    // Button focus
    {8, COLOR_BLACK, COLOR_GREEN, A_REVERSE},
    // List widget
    {9, COLOR_WHITE, COLOR_BLACK, A_NORMAL},
    // List widget focus
    {10, COLOR_BLACK, COLOR_GREEN, A_REVERSE},
    // Check Box Group
    {11, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Check Box Group Title
    {12, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // The marker of the end
    {0, 0, 0, 0}
};
#else
//
// Don't ask, my wife insisted... ;)
//
color Colors::__colors[] = {
    // Normal
    {1, COLOR_BLUE, COLOR_CYAN, A_NORMAL},
    // Message box title
    {2, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // Message box
    {3, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Input widget no focus
    {4, COLOR_BLACK, COLOR_WHITE, A_REVERSE},
    // Input widget focus
    {5, COLOR_WHITE, COLOR_MAGENTA, A_REVERSE},
    // Input widget text hidden (Only works with colors)
    {6, COLOR_MAGENTA, COLOR_MAGENTA, A_REVERSE},
    // Button no focus
    {7, COLOR_BLUE, COLOR_CYAN, A_NORMAL},
    // Button focus
    {8, COLOR_CYAN, COLOR_MAGENTA, A_REVERSE},
    // List widget
    {9, COLOR_BLUE, COLOR_CYAN, A_NORMAL},
    // List widget focus
    {10, COLOR_CYAN, COLOR_BLUE, A_REVERSE},
    // Check Box Group
    {11, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Check Box Group Title
    {12, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // The marker of the end
    {0, 0, 0, 0}
};
#endif
void
Colors::init_colors() {
    if (__initialized) return;

    if (has_colors() == FALSE) {
	__initialized = true;
	return;
    }

    start_color();
    assert (__colors[CHECKBOXGROUP_TITLE+1].no == 0);

    for (int i = 0; __colors[i].no != 0; i++)
	init_pair (__colors[i].no,
		   __colors[i].fg,
		   __colors[i].bg);

    __initialized = true;
}

void
Colors::set_color (WINDOW* w, COLORS c) {
    if (!__initialized)
	throw ColorsNotInitialized();

    if (has_colors() == TRUE) {
#if NCURSES_VERSION_PATCH < 20100313
        wattrset(w, COLOR_PAIR (__colors[c].no));
#else
	if (wattrset(w, COLOR_PAIR (__colors[c].no))==ERR)
	    throw CursesException("wattrset");
#endif
    } else {
#if NCURSES_VERSION_PATCH < 20100313
        wattrset(w, __colors[c].attr);
#else
	if (wattrset(w, __colors[c].attr)==ERR)
	    throw CursesException("wattrset");
#endif
    }
}

void
Colors::set_bg(WINDOW* w, COLORS c) {
   if (!__initialized)
	throw ColorsNotInitialized();

    if (has_colors() == TRUE)
	wbkgd(w, ' ' | COLOR_PAIR (__colors[c].no) );
    else
	wbkgd(w, ' ' | __colors[c].attr);
}

short
Colors::get_color (COLORS c) {
    if (has_colors() == TRUE) {
	return __colors[c].no;
    }

    return 0;
}
