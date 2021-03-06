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

#include "dynlabel.h"
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
DynLabel::DynLabel(const std::string& l) : Label(l) {}

DynLabel::~DynLabel() {}

void DynLabel::label(const std::string& l) {
    _label = l;

    if (realization() == REALIZED) refresh(true);
}

const std::string& DynLabel::label() const { return _label; }

void DynLabel::size_available(const Size& s) {
    assert(s.rows() > 0);
    WidgetBase::size_available(s);
    _size = Size(1, s.cols());
}

Size DynLabel::size_hint() const { return Size(1, 0); }

void DynLabel::reset_size() { _size = Size::zero(); }

void DynLabel::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    assert(widget_subwin() != 0);

    // Make sure there are no left overs in case of text being set by
    // a call to label() case we're realized.

    widget_subwin()->erase();
    CurStr tmp(_label, Coordinates(), color());
    widget_subwin()->addstrx(tmp);

    Widget::refresh(immediate);
}
