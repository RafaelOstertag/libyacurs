// $Id$

#include "coordinates.h"

//
// Private
//

//
// Protected
//

//
// Public
//

Coordinates::Coordinates() : __y(0), __x(0) {}

Coordinates::Coordinates(int _x, int _y) : __y(_y), __x(_x) {}

Coordinates::Coordinates(const Coordinates& _c) {
    __x = _c.__x;
    __y = _c.__y;
}

Coordinates::~Coordinates() {}

int
Coordinates::x() const {
    return __x;
}

int
Coordinates::y() const {
    return __y;
}

void
Coordinates::x(int _x) {
     __x = _x;
}

void
Coordinates::y(int _y) {
    __y = _y;
}

Coordinates&
Coordinates::operator=(const Coordinates& rhs) {
    __x = rhs.__x;
    __y = rhs.__y;
    return *this;
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

Coordinates
Coordinates::operator+(const Coordinates& rhs) const {
    Coordinates tmp = *this;
    return tmp+=rhs;
}

Coordinates
Coordinates::operator-(const Coordinates& rhs) const {
    Coordinates tmp = *this;
    return tmp-=rhs;
}

bool
Coordinates::operator==(const Coordinates& rhs) const {
    return __x == rhs.__x && __y == rhs.__y;
}

bool
Coordinates::operator!=(const Coordinates& rhs) const {
    return !operator==(rhs);
}