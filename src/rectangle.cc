// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#include "rectangle.h"

//
// Private
//

//
// Protected
//

//
// Public
//

Rectangle::Rectangle() : Coordinates(), c2() {}
Rectangle::Rectangle(int _y, int _x, int _nrows, int _ncols) :
    Coordinates(_x, _y),
    c2(_x + _ncols - 1, /* Transform to coords */
       _y + _nrows - 1 /* Transform to coords */)  {
    assert(this->x()>=0);
    assert(this->y()>=0);
    assert(c2.x()>=0);
    assert(c2.y()>=0);
}

Rectangle::Rectangle(const Coordinates& c, int _nrows, int _ncols) :
    Coordinates(c),
    c2(c.x()+_ncols-1,c.y()+_nrows-1) {
    assert(c2.x()>=0);
    assert(c2.y()>=0);
}

Rectangle::Rectangle(const Rectangle& d) : Coordinates(d), c2(d.c2) {
    assert(this->x()>=0);
    assert(this->y()>=0);
    assert(c2.x()>=0);
    assert(c2.y()>=0);
}

Rectangle::~Rectangle() {}

void
Rectangle::rows(int l) {
    assert(l>0);
    c2.y(this->y() + l-1);
}

void
Rectangle::cols(int c)  {
    assert(c>0);
    c2.x(this->x() + c-1);
}

int
Rectangle::rows() const {
    return (c2 - *this).y() + 1;
}

int
Rectangle::cols() const {
    return (c2 - *this).x() + 1;
}

Rectangle&
Rectangle::operator=(const Rectangle &d) {
    c2 = d.c2;
    Coordinates::operator=(d);

    return *this;
}

bool
Rectangle::operator==(const Rectangle& d) const {
    if (this == &d) return true;

    return Coordinates::operator==(*this) && c2 == d.c2;
}

bool
Rectangle::operator!=(const Rectangle& d) const {
    return ! operator==(d);
}

bool
Rectangle::operator==(const Coordinates& d) const {
    return false;
}

Rectangle&
Rectangle::operator-=(const Margin& rhs) {
    c2.y(c2.y() - rhs.bottom());
    c2.x(c2.x() - rhs.right());
    y(this->y() + rhs.top());
    x(this->x() + rhs.left());
    return *this;
}

Rectangle
Rectangle::operator-(const Margin& rhs) const {
    Rectangle tmp = *this;
    return tmp-=rhs;
}
