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
#include <stdexcept>

#include "size.h"

using namespace YACURS;

//
// Static
//
Size Size::__zero(0, 0);

//
// Private
//

//
// Protected
//

//
// Public
//

Size::Size(int16_t _rows, int16_t _cols) : __rows(_rows), __cols(_cols) {
    if (__rows < 0)
        throw std::out_of_range(_("Rows cannot be <0") );
    if (__cols < 0)
        throw std::out_of_range(_("Columns cannot be <0") );
}

int16_t
Size::cols() const {
    return __cols;
}

int16_t
Size::rows() const {
    return __rows;
}

void
Size::cols(int16_t _cols) {
    if (_cols < 0)
        throw std::out_of_range(_("Columns cannot be <0") );
    __cols = _cols;
}

void
Size::rows(int16_t _rows) {
    if (_rows < 0)
        throw std::out_of_range(_("Rows cannot be <0") );
    __rows = _rows;
}

const Size&
Size::operator+=(const Size& rhs) {
    assert(rhs.__cols >= 0);
    assert(rhs.__rows >= 0);
    __cols += rhs.__cols;
    __rows += rhs.__rows;
    return *this;
}

const Size&
Size::operator-=(const Size& rhs) {
    __cols -= rhs.__cols;
    if (__cols < 0) __cols = 0;

    __rows -= rhs.__rows;
    if (__rows < 0) __rows = 0;
    return *this;
}

bool
Size::operator==(const Size& rhs) const {
    return __cols == rhs.__cols && __rows == rhs.__rows;
}

const Size&
Size::zero() {
    return __zero;
}

Size
YACURS::operator+(const Size& lhs, const Size& rhs) {
    Size tmp = lhs;

    return tmp += rhs;
}

Size
YACURS::operator-(const Size& lhs, const Size& rhs) {
    Size tmp = lhs;

    return tmp -= rhs;
}

bool
YACURS::operator!=(const Size& lhs, const Size& rhs) {
    return !(lhs == rhs);
}
