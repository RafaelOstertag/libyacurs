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

#include "curstr.h"

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

CurStr::CurStr(const std::string& str, const Coordinates& pos, COLOROBJ c)
    : std::string{str}, _position{pos}, _color{c} {}

CurStr::CurStr(const std::string& str, int y, int x, COLOROBJ c)
    : std::string{str}, _position{Coordinates(x, y)}, _color{c} {}

CurStr::CurStr(const CurStr& cs)
    : std::string{cs}, _position{cs._position}, _color{cs._color} {}

CurStr& CurStr::operator=(const CurStr& cs) {
    if (this == &cs) return *this;
    std::string::operator=(cs);

    _position = cs._position;
    _color = cs._color;

    return *this;
}

CurStr::CurStr(CurStr&& cs)
    : std::string{std::move(cs)},
      _position{std::move(cs._position)},
      _color{cs._color} {}

CurStr& CurStr::operator=(CurStr&& cs) {
    if (this == &cs) return *this;
    std::string::operator=(cs);

    _position = std::move(cs._position);
    _color = cs._color;

    return *this;
}

void CurStr::position(const YACURS::Coordinates& co) { _position = co; }

const YACURS::Coordinates& CurStr::position() const { return _position; }

void CurStr::color(YACURS::COLOROBJ co) { _color = co; }

YACURS::COLOROBJ CurStr::color() const { return _color; }
