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

#include "rule.h"
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
Rule::Rule() : Widget(), _color(DEFAULT) {}

Rule::~Rule() {}

void Rule::color(COLOROBJ c) { _color = c; }

COLOROBJ
Rule::color() const { return _color; }

Size Rule::size() const { return _size; }

bool Rule::size_change() {
    // We don't handle size changes since we're not a container
    // Widget.
    return false;
}

void Rule::reset_size() { _size = Size::zero(); }
