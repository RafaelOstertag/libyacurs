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

#include <cassert>
#include <cstdlib>

#include "spacer.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

Spacer&
Spacer::operator=(const Spacer&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}


//
// Protected
//

//
// Private
//
Spacer::Spacer() {}

Spacer::~Spacer() {}

bool
Spacer::size_change() {
    return false;
}

Size
Spacer::size() const {
    return Size(1,1);
}

Size
Spacer::size_hint() const {
    return Size(1,1);
}

void
Spacer::reset_hint() {
    // Intentionally empty, since reset_size() is intended for
    // dynamically sized Widgets.
}

void
Spacer::reset_size() {
    // Intentionally empty, since reset_size() is intended for
    // dynamically sized Widgets.
}

bool
Spacer::can_focus() const {
    return false;
}

void
Spacer::refresh(bool) {}

void
Spacer::resize(const Area&) {}

void
Spacer::realize() {}

