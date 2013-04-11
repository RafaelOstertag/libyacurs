/*
 * This file is part of libyacurs, 
 * Copyright (C) 2013  Rafael Ostertag
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

// $Id$

#include "coordinates.h"

using namespace YACURS;

//
// Static
//
Coordinates Coordinates::__zero(0,0);

//
// Private
//

//
// Protected
//

//
// Public
//
Coordinates::Coordinates(int16_t _x, int16_t _y): __y(_y), __x(_x){
}

int16_t
Coordinates::x() const {
    return __x;
}

int16_t
Coordinates::y() const {
    return __y;
}

void
Coordinates::x(int16_t _x) {
     __x = _x;
}

void
Coordinates::y(int16_t _y) {
    __y = _y;
}

Coordinates&
Coordinates::operator+=(const Coordinates& rhs) {
    __x += rhs.__x;
    __y += rhs.__y;
    return *this;
}

Coordinates&
Coordinates::operator-=(const Coordinates& rhs) {
    __x -= rhs.__x;
    __y -= rhs.__y;
    return *this;
}

bool
Coordinates::operator==(const Coordinates& rhs) const {
    return __x == rhs.__x && __y == rhs.__y;
}

const Coordinates&
Coordinates::zero() {
    return __zero;
}

Coordinates
YACURS::operator+(const Coordinates& lhs, const Coordinates& rhs) {
    Coordinates tmp = lhs;
    return tmp+=rhs;
}

Coordinates
YACURS::operator-(const Coordinates& lhs, const Coordinates& rhs) {
    Coordinates tmp = lhs;
    return tmp-=rhs;
}

bool
YACURS::operator!=(const Coordinates& lhs, const Coordinates& rhs) {
    return !(lhs==rhs);
}
