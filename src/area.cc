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

Area::~Area() {}

bool
Area::operator==(const Area& d) const {
    return Coordinates::operator==(*this) && 
	Size::operator==(*this);
}

bool
Area::operator!=(const Area& d) const {
    return !operator==(d);
}

Area&
Area::operator-=(const Margin& rhs) {
    rows(rows() - rhs.bottom());
    cols(cols() - rhs.right());
    y(y() + rhs.top());
    x(x() + rhs.left());
    return *this;
}

Area
Area::operator-(const Margin& rhs) const {
    Area tmp = *this;
    return tmp-=rhs;
}
