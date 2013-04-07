// $Id$

#include "coordinates.h"

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
operator+(const Coordinates& lhs, const Coordinates& rhs) {
    Coordinates tmp = lhs;
    return tmp+=rhs;
}

Coordinates
operator-(const Coordinates& lhs, const Coordinates& rhs) {
    Coordinates tmp = lhs;
    return tmp-=rhs;
}

bool
operator!=(const Coordinates& lhs, const Coordinates& rhs) {
    return !(lhs==rhs);
}
