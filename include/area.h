// -*- mode: c++ -*-
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

#ifndef AREA_H
#define AREA_H 1

#include <string>

#include "coordinates.h"
#include "margin.h"
#include "size.h"

namespace YACURS {
/**
 * Describes an area on the screen.
 *
 * An area is described as starting point on the screen and number of
 * lines and columns of extent.
 */
class Area : public Coordinates, public Size {
   private:
    static Area _zero;

   public:
    /**
     * Constructor.
     *
     * @param y value of y component
     *
     * @param x value of x component
     *
     * @param rows number of rows from (x/y)
     *
     * @param cols number of columns from (x/y)
     */
    Area(int y = 0, int x = 0, int rows = 0, int cols = 0);

    /**
     * Initialize Area with Coordinates and Size.
     *
     * @param c reference to Coordinates object.
     *
     * @param s reference to Size object.
     */
    Area(const Coordinates& c, const Size& s);

    /**
     * Copy constructor.
     *
     * @param d reference to Area object.
     */
    Area(const Area& d);

    /**
     * Assign another Area object.
     *
     * @param a reference to Area object.
     *
     * @return reference to @c this.
     */
    Area& operator=(const Area& a);

    /**
     * Assign Size object.
     *
     * Only the Size part of Area will be affected. The
     * Coordinates part remains untouched.
     *
     * @param s reference to Size object.
     *
     * @return reference to @c this.
     */
    Area& operator=(const Size& s);

    /**
     * Assign Coordinates object.
     *
     * Only the Coordinates part of Area will be affected. The
     * Size component remains untouched.
     *
     * @param c reference to Coordinates object.
     *
     * @return reference to @c this.
     */
    Area& operator=(const Coordinates& c);

    /**
     * Test two Area object for equality.
     *
     * Test whether Size and Position are equal.
     *
     * @param a right hand side.
     *
     * @return @c true if both objects are of identical
     * dimension. @c false otherwise.
     */
    bool operator==(const Area& a) const;

    bool operator==(const Size& s) const;

    bool operator==(const Coordinates& c) const;

    /**
     * Subtract Margin from Area.
     *
     * @param rhs right hand side of type Margin
     *
     * @return const reference to the result.
     */
    Area& operator-=(const Margin& rhs);

    /**
     * Coordinates of the lower right corner.
     *
     * @return coordinates of the lower right corner.
     */
    Coordinates end() const;

    static const Area& zero();
};

/**
 * Test two Area object for inequality.
 *
 * @param a right hand side.
 *
 * @return @c true if objects are not equal, @c false
 * otherwise.
 */
bool operator!=(const Area& lhs, const Area& a);

/**
 * Subtract Margin from Area.
 *
 * @param rhs right hand side of type Margin.
 *
 * @return result, i.e. Area-Margin.
 */
Area operator-(const Area& lhs, const Margin& rhs);

/**
 * Test whether coordinates lie within area.
 */
bool operator>(const Area& lhs, const Coordinates& rhs);

/**
 * Test whether coordinates lie within area.
 */
bool operator<(const Coordinates& lhs, const Area& rhs);

inline Area::Area(int y, int x, int rows, int cols)
    : Coordinates(x, y), Size(rows, cols) {}

inline Area::Area(const Coordinates& c, const Size& s)
    : Coordinates(c), Size(s) {}

inline Area::Area(const Area& a) : Coordinates(a), Size(a) {}

inline Area& Area::operator=(const Area& a) {
    Coordinates::operator=(a);
    Size::operator=(a);

    return *this;
}

inline Area& Area::operator=(const Size& s) {
    Size::operator=(s);

    return *this;
}

inline Area& Area::operator=(const Coordinates& c) {
    Coordinates::operator=(c);

    return *this;
}

inline bool Area::operator==(const Area& a) const {
    return Coordinates::operator==(a) && Size::operator==(a);
}

inline bool Area::operator==(const Size& s) const {
    return Size::operator==(s);
}

inline bool Area::operator==(const Coordinates& c) const {
    return Coordinates::operator==(c);
}

inline Area& Area::operator-=(const Margin& rhs) {
    y(y() + rhs.top());
    x(x() + rhs.left());
    rows(rows() - rhs.bottom() - rhs.top());
    cols(cols() - rhs.right() - rhs.left());

    return *this;
}

inline Coordinates Area::end() const {
    return Coordinates(x() + cols(), y() + rows());
}

inline bool operator!=(const Area& lhs, const Area& a) { return !(lhs == a); }

inline Area operator-(const Area& lhs, const Margin& rhs) {
    Area tmp = lhs;

    return tmp -= rhs;
}

inline bool operator>(const Area& lhs, const Coordinates& rhs) {
    return lhs.x() <= rhs.x() && lhs.cols() + lhs.x() >= rhs.x() &&
           lhs.y() <= rhs.y() && lhs.rows() + lhs.y() >= rhs.y();
}

inline bool operator<(const Coordinates& lhs, const Area& rhs) {
    return rhs > lhs;
}

inline const Area& Area::zero() { return Area::_zero; }
}  // namespace YACURS

#endif
