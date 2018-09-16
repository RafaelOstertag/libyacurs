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

#include <cassert>
#include <cerrno>
#include <cstdlib>

#include "label.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//


size_t Label::label_length() const {
#ifdef YACURS_USE_WCHAR
    size_t mbslen = std::mbstowcs(0, _label.c_str(), 0);
    if (mbslen == (size_t)-1) throw EXCEPTIONS::SystemError(errno);

    return mbslen;
#else
    return _label.length();
#endif
}

//
// Protected
//

//
// Public
//
Label::Label(const std::string& l)
    : Widget(),
      _color(DEFAULT),
      _label(l),
      _size(Size(1, label_length())) {}

Label::~Label() {}

void Label::label(const std::string& l) {
    _label = l;

    Size oldsize = _size;

    _size = Size(1, label_length());

    // If parent is 0, we have nobody to inform about a possible
    // size change. If old size is the same as the new size, we simply
    // have to refresh immediately.
    //
    // In any case, we can leave the function.
    if (parent() == 0 || oldsize.cols() >= _size.cols()) {
        if (realization() == REALIZED) refresh(true);
        return;
    }

    // The size has changed notify parent of size change
    parent()->size_change();
}

const std::string& Label::label() const { return _label; }

Size Label::size() const { return _size; }

Size Label::size_hint() const { return _size; }

bool Label::size_change() {
    // We don't handle size changes since we're not a container
    // Widget.
    return false;
}

void Label::reset_size() {
    // Intentionally empty, since reset_size() is intended for
    // dynamically sized Widgets.
}

void Label::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    assert(widget_subwin() != 0);

    widget_subwin()->erase();
    CurStr tmp(_label, Coordinates(), color());
    widget_subwin()->addstr(tmp);

    Widget::refresh(immediate);
}

void Label::color(COLOROBJ c) { _color = c; }

COLOROBJ
Label::color() const { return _color; }
