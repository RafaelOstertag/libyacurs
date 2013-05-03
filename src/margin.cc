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

#include "algorithm"

#include "margin.h"

using namespace YACURS;

//
// Static
//
Margin Margin::__zero(0, 0, 0, 0);

//
// Private
//

//
// Protected
//

//
// Public
//
Margin::Margin(int _top, int _left, int _bottom, int _right) :
    __top(_top),
    __bottom(_bottom),
    __left(_left),
    __right(_right) {
}

Margin::Margin(const Margin& m) :
    __top(m.__top),
    __bottom(m.__bottom),
    __left(m.__left),
    __right(m.__right) {
}

Margin::~Margin() {
}

void
Margin::top(int i) {
    __top = i;
}

void
Margin::bottom(int i) {
    __bottom = i;
}

void
Margin::left(int i) {
    __left = i;
}

void
Margin::right(int i) {
    __right = i;
}

int
Margin::top() const {
    return __top;
}

int
Margin::bottom() const {
    return __bottom;
}

int
Margin::left() const {
    return __left;
}

int
Margin::right() const {
    return __right;
}

Margin&
Margin::operator=(const Margin& m) {
    __top = m.__top;
    __bottom = m.__bottom;
    __left = m.__left;
    __right = m.__right;
    return *this;
}

Margin
Margin::operator+(const Margin& m) const {
    Margin tmp = *this;

    tmp.__top = std::max(__top, m.__top);
    tmp.__bottom = std::max(__bottom, m.__bottom);
    tmp.__left = std::max(__left, m.__left);
    tmp.__right = std::max(__right, m.__right);

    return tmp;
}

Margin
Margin::operator-(const Margin& m) const {
    Margin tmp = *this;

    tmp.__top = std::min(__top, m.__top);
    tmp.__bottom = std::min(__bottom, m.__bottom);
    tmp.__left = std::min(__left, m.__left);
    tmp.__right = std::min(__right, m.__right);

    return tmp;
}

bool
Margin::operator==(const Margin& m) const {
    if (this == &m) return true;

    return __top == m.__top &&
           __bottom == m.__bottom &&
           __left == m.__left &&
           __right == m.__right;
}

bool
Margin::operator!=(const Margin& m) const {
    return !operator==(m);
}

const Margin&
Margin::zero() {
    return __zero;
}
