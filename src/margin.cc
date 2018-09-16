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
Margin Margin::_zero(0, 0, 0, 0);

//
// Private
//

//
// Protected
//

//
// Public
//
Margin::Margin(int top, int left, int bottom, int right)
    : _top(top), _bottom(bottom), _left(left), _right(right) {}

Margin::~Margin() {}

void Margin::top(int i) { _top = i; }

void Margin::bottom(int i) { _bottom = i; }

void Margin::left(int i) { _left = i; }

void Margin::right(int i) { _right = i; }

int Margin::top() const { return _top; }

int Margin::bottom() const { return _bottom; }

int Margin::left() const { return _left; }

int Margin::right() const { return _right; }

Margin Margin::operator+(const Margin& m) const {
    Margin tmp = *this;

    tmp._top = std::max(_top, m._top);
    tmp._bottom = std::max(_bottom, m._bottom);
    tmp._left = std::max(_left, m._left);
    tmp._right = std::max(_right, m._right);

    return tmp;
}

Margin Margin::operator-(const Margin& m) const {
    Margin tmp = *this;

    tmp._top = std::min(_top, m._top);
    tmp._bottom = std::min(_bottom, m._bottom);
    tmp._left = std::min(_left, m._left);
    tmp._right = std::min(_right, m._right);

    return tmp;
}

bool Margin::operator==(const Margin& m) const {
    if (this == &m) return true;

    return _top == m._top && _bottom == m._bottom && _left == m._left &&
           _right == m._right;
}

bool Margin::operator!=(const Margin& m) const { return !operator==(m); }

const Margin& Margin::zero() { return _zero; }
