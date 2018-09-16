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

#include "vrule.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
VRule& VRule::operator=(const VRule&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//
VRule::VRule() : Rule() {}

VRule::~VRule() {}

void VRule::size_available(const Size& _s) {
    assert(_s.cols() > 0);
    WidgetBase::size_available(_s);
    __size = Size(_s.rows(), 1);
}

Size VRule::size_hint() const { return Size(0, 1); }

void VRule::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    assert(widget_subwin() != 0);

    widget_subwin()->set_color(__color);
    widget_subwin()->set_bg(__color);
    widget_subwin()->vrule();

    Widget::refresh(immediate);
}
