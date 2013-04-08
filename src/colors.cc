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

    // If colors are not supported, we're done, since we rely on
    // attributes solely.
    if (has_colors() == FALSE) {
	__initialized = true;
	return;
    }

    if (start_color()==ERR)
	throw CursesException("start_color");

    __initialized = true;

    if (COLOR_PAIRS < NUMBER_OF_COLOROBJ)
	return;

    for (int i = 0; i < NUMBER_OF_COLOROBJ ; i++) {
	if (init_pair (__colors.at(i).no,
		       __colors.at(i).fg,
		       __colors.at(i).bg)==ERR)
	    throw CursesException("init_pair");
    }
}

int
Colors::color_pair(COLOROBJ c) {
    if (!__initialized)
	throw ColorsNotInitialized();

    if (has_colors() == TRUE && COLOR_PAIRS >= NUMBER_OF_COLOROBJ) {
        return COLOR_PAIR (__colors[c].no);
    } else {
        return __colors[c].attr;
    }
}
