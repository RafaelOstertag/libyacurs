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

CurStr::CurStr(const std::string& str, const Coordinates& pos, COLOROBJ c) :
    std::string(str),
    __position(pos),
    __color(c) {
}

CurStr::CurStr(const std::string& str, int y, int x, COLOROBJ c) :
    std::string(str),
    __position(Coordinates(x, y) ),
    __color(c) {
}

CurStr::CurStr(const CurStr& cs) : std::string(cs),
    __position(cs.__position),
    __color(cs.__color) {
}

CurStr&
CurStr::operator=(const CurStr& cs) {
    if (this == &cs) return *this;
    std::string::operator=(cs);

    __position = cs.__position;
    __color = cs.__color;

    return *this;
}

void
CurStr::position(const YACURS::Coordinates& co) {
    __position = co;
}

const YACURS::Coordinates&
CurStr::position() const {
    return __position;
}

void
CurStr::color(YACURS::COLOROBJ co) {
    __color = co;
}

YACURS::COLOROBJ
CurStr::color() const {
    return __color;
}
