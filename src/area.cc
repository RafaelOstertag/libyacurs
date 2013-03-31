// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#include "area.h"

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

bool
operator!=(const Area& lhs, const Area& _a) {
    return !(lhs==_a);
}

Area
operator-(const Area& lhs, const Margin& rhs) {
    Area tmp = lhs;
    return tmp-=rhs;
}

const Area&
Area::zero() {
    return Area::__zero;
}
