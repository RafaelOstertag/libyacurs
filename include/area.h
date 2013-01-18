// -*- mode: c++ -*-

#ifndef AREA_H
#define AREA_H

#include "coordinates.h"
#include "size.h"
#include "margin.h"

/**
 * Describes an area on the screen.
 *
 * The area is described as starting point on the screen and number of
 * lines and columns of extent.
 */
class Area: public Coordinates, public Size {
    public:
	Area(int _y=0, int _x=0, int _rows=0, int _cols=0);
	Area(const Coordinates& _c, const Size& _s);
	Area(const Area& d);

	Area& operator=(const Area &_a);
	Area& operator=(const Size &_s);
	Area& operator=(const Coordinates &_c);

	bool operator==(const Area& _a) const;
	bool operator!=(const Area& _a) const;
	const Area& operator-=(const Margin& rhs);
	Area operator-(const Margin& rhs) const;
};

#endif
