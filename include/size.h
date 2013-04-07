// -*- mode: c++ -*-
//
// $Id$

#ifndef SIZE_H
#define SIZE_H

#ifdef HAVE_CSTDINT
# include <cstdint>
#else // HAVE_CSTDINT
# ifdef HAVE_STDINT_H
#  include <stdint.h>
# endif // HAVE_STDINT_H
#endif // HAVE_CSTDINT

#include <string>

/**
 * Describes size in terms of lines and columns.
 *
 * Neither lines nor columns may be less than 0. When subtracting two
 * sizes object, and either component would be smaller than 0, the
 * value in question is silently set to 0.
 *
 */
class Size {
    private:
	/**
	 * Object representing zero.
	 */
	static Size __zero;
	int16_t __rows, __cols;

    public:
	/**
	 * Constructor.
	 *
	 * When no arguments are provided, it initializes the object
	 * to zero, i.e. rows=, lines=0.
	 *
	 * @param _rows amount of rows
	 * @param _cols amount of columns.
	 */
	Size(int16_t _rows=0, int16_t _cols=0);

	/**
	 * Get the colums.
	 *
	 * @return the number of columns.
	 */
	int16_t cols() const;

	/**
	 * Get the rows.
	 *
	 * @return the number of rows.
	 */
	int16_t rows() const;

	/**
	 * Set the number of columns.
	 *
	 * If number of columns is tried to set to a negative value, an
	 * out_of_range exception is thrown.
	 *
	 * @param _cols number of columns
	 */
	void cols(int16_t _cols);

	/**
	 * Set the number of rows.
	 *
	 * If number of rows is tried to set to a negative value, an
	 * out_of_range exception is thrown.
	 *
	 * @param _rows number of rows
	 */
	void rows(int16_t _rows);

	/**
	 * Add Size to this object.
	 *
	 * Addition will be performed component wise, i.e. rows +
	 * rows, columns + columns.
	 *
	 * @param rhs Size to add.
	 *
	 * @returns reference to this.
	 */
	const Size& operator+=(const Size& rhs);

	/**
	 * Subtract Size from this object.
	 *
	 * Subtraction will be performed component wise, i.e. rows -
	 * rows, columns - columns. If either resulting component
	 * would be less than 0, it will be silently set to 0.
	 *
	 * @param rhs Size to subtract.
	 *
	 * @returns reference to this.
	 */
	const Size& operator-=(const Size& rhs);

	/**
	 * Test two Size objects for equality.
	 *
	 * @param rhs right hand side
	 *
	 * @return @c true if lines and rows are equal, @c false
	 * otherwise.
	 */
	bool operator==(const Size& rhs) const;

	/**
	 * Return the object representing zero.
	 *
	 * @return Size object representing zero.
	 */
	static const Size& zero();
};

/**
 * Add Size to this object.
 *
 * Addition will be performed component wise, i.e. rows +
 * rows, columns + columns.
 *
 * @param rhs Size to add.
 *
 * @returns size object holding sum
 */
Size operator+(const Size& lhs, const Size& rhs);

/**
 * Subtract this from Size object
 *
 * Subtraction will be performed component wise, i.e. rows -
 * rows, columns - columns. If either resulting component
 * would be less than 0, it will be silently set to 0.
 *
 * @param rhs Size object
 *
 * @returns size object holding difference
 */
Size operator-(const Size& lhs, const Size& rhs);

/**
 * Test two Size objects for inequality.
 *
 * @param rhs right hand side.
 *
 * @return @c true if lines or rows are not equal, @c false
 * otherwise.
 */
bool operator!=(const Size& lhs, const Size& rhs);

#endif // SIZE_H
