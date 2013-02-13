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
	 * Constructor.
	 *
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
	 * Size component remains untouched.
	 *
	 * @param _c reference to Coordinates object.
	 *
	 * @return reference to @c this.
	 */
	Area& operator=(const Coordinates &_c);

	/**
	 * Test two Area object for equality.
	 *
	 * Test whether Size and Position are equal.
	 *
	 * @param _a right hand side.
	 *
	 * @return @c true if both objects are of identical
	 * dimension. @c false otherwise.
	 */
	bool operator==(const Area& _a) const;

	/**
	 * Test two Area object for inequality.
	 *
	 * @param _a right hand side.
	 *
	 * @return @c true if objects are not equal, @c false
	 * otherwise.
	 */
	bool operator!=(const Area& _a) const;

	/**
	 * Subtract Margin from Area.
	 *
	 * @param rhs right hand side of type Margin
	 *
	 * @return const reference to the result.
	 */
	const Area& operator-=(const Margin& rhs);

	/**
	 * Subtract Margin from Area.
	 *
	 * @param rhs right hand side of type Margin.
	 *
	 * @return result, i.e. Area-Margin.
	 */
	Area operator-(const Margin& rhs) const;

	/**
	 * Convert to std::string.
	 *
	 * Convert the object to a std::string. Used for debugging.
	 */
	operator std::string() const;
};

#endif
