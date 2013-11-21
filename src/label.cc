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
Label&
Label::operator=(const Label&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

size_t
Label::label_length() const {
#ifdef YACURS_USE_WCHAR
    size_t mbslen = mbstowcs(NULL, __label.c_str(), 0);
    if (mbslen == (size_t)-1)
        throw EXCEPTIONS::SystemError(errno);

    return mbslen;
#else
    return __label.length();
#endif
}

//
// Protected
//

//
// Public
//
Label::Label(const std::string& _l) : 
    Widget(),
    __color(DEFAULT),
    __label(_l),
    __size(Size(1, label_length() ) ) {
}

Label::~Label() {
}

void
Label::label(const std::string& _l) {
    __label = _l;

    Size oldsize = __size;

    __size = Size(1, label_length() );

    // If parent is 0, we have nobody to inform about a possible
    // size change. If old size is the same as the new size, we simply
    // have to refresh immediately.
    //
    // In any case, we can leave the function.
    if (parent() == 0 || oldsize.cols() >= __size.cols() ) {
        if (realization() == REALIZED) refresh(true);
        return;
    }

    // The size has changed notify parent of size change
    parent()->size_change();
}

const std::string&
Label::label() const {
    return __label;
}

Size
Label::size() const {
    return __size;
}

Size
Label::size_hint() const {
    return __size;
}

bool
Label::size_change() {
    // We don't handle size changes since we're not a container
    // Widget.
    return false;
}

void
Label::reset_size() {
    // Intentionally empty, since reset_size() is intended for
    // dynamically sized Widgets.
}

void
Label::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    assert(widget_subwin() != 0);

    widget_subwin()->erase();
    CurStr tmp(__label, Coordinates(), color() );
    widget_subwin()->addstr(tmp);

    Widget::refresh(immediate);
}

void
WidgetBase::unrealize() {
    UNREALIZE_ENTER;

    // Intentionally empty

    UNREALIZE_LEAVE;
}

void
Label::color(COLOROBJ c) {
    __color = c;
}

COLOROBJ
Label::color() const {
    return __color;
}
