//
// This file is part of libyacurs, 
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

#include "area.h"

using namespace YACURS;

//
// Private
//
Area Area::__zero(0,0,0,0);

//
// Protected
//

//
// Public
//

Area::Area(int _y, int _x, int _rows, int _cols):
    Coordinates(_x, _y), Size(_rows, _cols) {
}

Area::Area(const Coordinates& _c, const Size& _s):
    Coordinates(_c), Size(_s) {
}

Area::Area(const Area& _a): Coordinates(_a), Size(_a) {
}

Area&
Area::operator=(const Area& _a) {
    Coordinates::operator=(_a);
    Size::operator=(_a);
    return *this;
}

Area&
Area::operator=(const Size& _s) {
    Size::operator=(_s);
    return *this;
}

Area&
Area::operator=(const Coordinates& _c) {
    Coordinates::operator=(_c);
    return *this;
}

bool
Area::operator==(const Area& _a) const {
    return Coordinates::operator==(_a) && 
	Size::operator==(_a);
}

bool
Area::operator==(const Size& _s) const {
    return Size::operator==(_s);
}

bool
Area::operator==(const Coordinates& _c) const {
    return Coordinates::operator==(_c);
}

Area&
Area::operator-=(const Margin& rhs) {
    y(y() + rhs.top());
    x(x() + rhs.left());
    rows(rows() - rhs.bottom() - rhs.top());
    cols(cols() - rhs.right() - rhs.left());

    return *this;
}

Coordinates
Area::end() const {
    return Coordinates(x()+cols(), y()+rows());
}

bool
YACURS::operator!=(const Area& lhs, const Area& _a) {
    return !(lhs==_a);
}

Area
YACURS::operator-(const Area& lhs, const Margin& rhs) {
    Area tmp = lhs;
    return tmp-=rhs;
}

bool 
YACURS::operator>(const Area& lhs, const Coordinates& rhs) {
    return lhs.x()<=rhs.x() && lhs.cols()+lhs.x()>=rhs.x() &&
	lhs.y()<=rhs.y() && lhs.rows()+lhs.y()>=rhs.y();
}

bool 
YACURS::operator<(const Coordinates& lhs, const Area& rhs) {
    return rhs>lhs;
}

const Area&
Area::zero() {
    return Area::__zero;
}
