//
// This file is part of libyacurs.
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <cassert>
#include <cstdlib>

#include "radiobox.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//

//
// Public
//

RadioBox::RadioBox(const std::string& title,
                   const std::vector<std::string>& items)
    : CheckBox(title, items) {
    _indicators[0] = "( ) ";
    _indicators[1] = "(o) ";
}

RadioBox::~RadioBox() {}

void RadioBox::set_selection(unsigned short cursor) {
    if (cursor >= _items.size())
        throw std::out_of_range(
            _("RadioBox cursor out of range in set_selection()"));

    for (std::vector<INTERNAL::Selectable>::size_type i = 0; i < _items.size();
         i++)
        _items[i].selected = false;

    _items[cursor].selected = true;
}

void RadioBox::set_selection(const std::string& str) {
    CheckBox::set_selection(str);
}
