// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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

#include <vector>
#include <cassert>

#include "colors.h"
#include "colorparser.h"
#include "yacursex.h"

using namespace YACURS;

bool Colors::__initialized = false;

std::vector<CursColor> Colors::__colors;

void
Colors::init_colors(const std::string& colorstr) {
    if (__initialized) return;

    ColorParser cp;

    if (colorstr.empty())
	__colors=cp();
    else
	__colors=cp(colorstr);

    if (has_colors() == FALSE || can_change_color() == FALSE ) {
	__initialized = true;
	return;
    }

    start_color();

    __initialized = true;

    if (COLORS <= NUMBER_OF_COLOROBJ)
	return;

    for (int i = 0; i < NUMBER_OF_COLOROBJ ; i++)
	init_pair (__colors.at(i).no,
		   __colors.at(i).fg,
		   __colors.at(i).bg);
}

void
Colors::set_color (WINDOW* w, COLOROBJ c) {
    if (!__initialized)
	throw ColorsNotInitialized();

    if (has_colors() == TRUE && COLORS >= NUMBER_OF_COLOROBJ) {
#if NCURSES_VERSION_PATCH < 20100313
        wattrset(w, COLOR_PAIR (__colors.at(c).no));
#else
	if (wattrset(w, COLOR_PAIR (__colors.at(c).no))==ERR)
	    throw CursesException("wattrset");
#endif
    } else {
#if NCURSES_VERSION_PATCH < 20100313
        wattrset(w, __colors.at(c).attr);
#else
	if (wattrset(w, __colors.at(c).attr)==ERR)
	    throw CursesException("wattrset");
#endif
    }
}

void
Colors::set_bg(WINDOW* w, COLOROBJ c) {
   if (!__initialized)
	throw ColorsNotInitialized();

    if (has_colors() == TRUE && COLORS >= NUMBER_OF_COLOROBJ)
	wbkgd(w, ' ' | COLOR_PAIR (__colors.at(c).no) );
    else
	wbkgd(w, ' ' | __colors.at(c).attr);
}

short
Colors::get_color (COLOROBJ c) {
    if (has_colors() == TRUE && COLORS >= NUMBER_OF_COLOROBJ) {
	return __colors.at(c).no;
    }

    return 0;
}
