// -*- mode: c++ -*-
//
// $Id$

#ifndef COORDINATES_H
#define COORDINATES_H

#include <string>

/**
 * Coordinates as used throughout Curses.
 */
class Coordinates {
    private:
	/**
	 * Object representing zero.
	 */
	static Coordinates __zero;
	/**
	 * y component.
	 */
	int __y;
	/**
	 * x component
	 */
	int __x;

    public:
	/**
	 * Constructor.
	 *
	 * When no arguments are provided, it initializes the object
	 * to zero, i.e. x=0, y=0.
	 *
	 * @param _x x-component
	 * @param _y y-component
	 */
	Coordinates(int _x=0, int _y=0);

	/**
	 * Get x component.
	 *
	 * @return x component
	 */
	int x() const;

	/**
	 * Get y component
	 *
	 * @return y component
	 */
	int y() const;

	/**
	 * Set x component.
	 *
	 * @param _x value of x
	 */
	void x(int _x);

	/**
	 * Set y component.
	 *
	 * @param _y value of y
	 */
	void y(int _y);

	/**
	 * Add and assign another Coordinates object.
	 *
	 * Component wise add and assign another Coordinates object.
	 *
	 * @param rhs right hand side to add
	 *
	 * @return reference to @c this.
	 */
	Coordinates& operator+=(const Coordinates& rhs);

	/**
	 * Subtract and assign another Coordinates object.
	 *
	 * Component wise subtract and assign another Coordinates
	 * object.
	 *
	 * @param rhs right hand side to subtract.
	 *
	 * @return reference to @c this.
	 */
	Coordinates& operator-=(const Coordinates& rhs);

	/**
	 * Component wise addition.
	 *
	 * Adds two Coordinates objects component wise.
	 *
	 * @param rhs right hand side of addition.
	 *
	 * @return sum of two Coordinate objects.
	 */
	Coordinates operator+(const Coordinates& rhs) const;

	/**
	 * Component wise subtraction.
	 *
	 * Subtracts two Coordinates objects comonent wise.
	 *
	 * @param rhs right hand side of subtraction.
	 *
	 * @return difference of two Coordinate objects.
	 */
	Coordinates operator-(const Coordinates& rhs) const;

	/**
	 * Test two Coordinates objects for equality.
	 *
	 * Test two Coordinates object component wise for equality.
	 *
	 * @param rhs right hand side to test for equality.
	 *
	 * @return @c true if both Coordinates objects are component
	 * wise equal, @c false otherwise.
	 */
	bool operator==(const Coordinates& rhs) const;

	/**
	 * Test two Coordinates object for inequality.
	 *
	 * Simply
	 *
	 *  <code>!Coordinates::operator==()</code>
	 *
	 * @param rhs right hand side of comparison.
	 *
	 * @return @c true if Coordinates object are not equal, @c
	 * false otherwise.
	 */
	bool operator!=(const Coordinates& rhs) const;

	/**
	 * Return the object representing zero.
	 *
	 * @return Coordinates object representing zero.
	 */
	static const Coordinates& zero();
};

#endif // COORDINATES_H
