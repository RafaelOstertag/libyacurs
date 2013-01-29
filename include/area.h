// -*- mode: c++ -*-

#ifndef AREA_H
#define AREA_H

#include <string>

#include "coordinates.h"
#include "size.h"
#include "margin.h"

/**
 * Describes an area on the screen.
 *
 * An area is described as starting point on the screen and number of
 * lines and columns of extent.
 */
class Area: public Coordinates, public Size {
    public:
	/**
	 * @param _y value of y component
	 *
	 * @param _x value of x component
	 *
	 * @param _rows number of rows from (x/y)
	 *
	 * @param _cols number of columns from (x/y)
	 */
	Area(int _y=0, int _x=0, int _rows=0, int _cols=0);

	/**
	 * Initialize Area with Coordinates and Size.
	 *
	 * @param _c reference to Coordinates object.
	 *
	 * @param _s reference to Size object.
	 */
	Area(const Coordinates& _c, const Size& _s);

	/**
	 * Copy constructor.
	 *
	 * @param d reference to Area object.
	 */
	Area(const Area& d);

	/**
	 * Assign another Area object.
	 *
	 * @param _a reference to Area object.
	 *
	 * @return reference to @c this.
	 */
	Area& operator=(const Area &_a);

	/**
	 * Assign Size object.
	 *
	 * Only the Size part of Area will be affected. The
	 * Coordinates part remains untouched.
	 *
	 * @param _s reference to Size object.
	 *
	 * @return reference to @c this.
	 */
	Area& operator=(const Size &_s);

	/**
	 * Assign Coordinates object.
	 *
	 * Only the Coordinates part of Area will be affected. The
	 * Size part remains untouched.
	 *
	 * @param _c reference to Coordinates object.
	 *
	 * @return reference to @c this.
	 */
	Area& operator=(const Coordinates &_c);

	/**
	 * Test two Area object for equality.
	 *
	 * Test two Area object for equality, i.e.
	 */
	bool operator==(const Area& _a) const;
	bool operator!=(const Area& _a) const;
	const Area& operator-=(const Margin& rhs);
	Area operator-(const Margin& rhs) const;

	operator std::string() const;
};

#endif
