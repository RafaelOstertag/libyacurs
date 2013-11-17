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
#include <cstdlib>

#include "hrule.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
HRule&
HRule::operator=(const HRule&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//
HRule::HRule() : 
    Widget(),
    __color(DEFAULT) {
}

HRule::~HRule() {
}

Size
HRule::size() const {
    return __size;
}

void
HRule::size_available(const Size& _s) {
    assert(_s.rows() > 0);
    WidgetBase::size_available(_s);
    __size = Size(1, _s.cols() );
}

Size
HRule::size_hint() const {
    return Size(1, 0);
}

bool
HRule::size_change() {
    // We don't handle size changes since we're not a container
    // Widget.
    return false;
}

void
HRule::reset_size() {
    __size = Size::zero();
}

void
HRule::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    assert(widget_subwin() != 0);

    widget_subwin()->set_color(__color);
    widget_subwin()->set_bg(__color);
    widget_subwin()->hrule();

    Widget::refresh(immediate);
}
