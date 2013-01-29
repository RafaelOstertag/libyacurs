// -*- mode: c++ -*-
//
// $Id$

#ifndef SIZE_H
#define SIZE_H

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
	int __rows, __cols;

    public:
	Size(int _rows=0, int _cols=0);
	Size(const Size& _s);

	/**
	 * Get the colums.
	 *
	 * @return the number of columns.
	 */
	int cols() const;
	/**
	 * Get the rows.
	 *
	 * @return the number of rows.
	 */
	int rows() const;
	/**
	 * Set the number of columns.
	 *
	 * If number of columns is tried to set to a negative value, an
	 * out_of_range exception is thrown.
	 *
	 * @param _cols number of columns
	 */
	void cols(int _cols);
	/**
	 * Set the number of rows.
	 *
	 * If number of rows is tried to set to a negative value, an
	 * out_of_range exception is thrown.
	 *
	 * @param _rows number of rows
	 */
	void rows(int _rows);

	const Size& operator=(const Size& rhs);
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
	 * Add Size to this object.
	 *
	 * Addition will be performed component wise, i.e. rows +
	 * rows, columns + columns.
	 *
	 * @param rhs Size to add.
	 *
	 * @returns size object holding sum
	 */
	Size operator+(const Size& rhs) const;
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
	Size operator-(const Size& rhs) const;
	bool operator==(const Size& rhs) const;
	bool operator!=(const Size& rhs) const;

	/**
	 * Return the object representing zero.
	 *
	 * @return Size object representing zero.
	 */
	static const Size& zero();

	operator std::string() const;
};

#endif // SIZE_H
