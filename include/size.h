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

#ifndef SIZE_H
#define SIZE_H 1

// Can't use #ifdef HAVE_STDINT_H since that would require pulling in
// config.h or libyacurscfg.h which might cause undesired side
// effects.
#include <stdint.h>

#include <cassert>
#include <stdexcept>
#include <string>

namespace YACURS {
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
    static Size _zero;
    int16_t _rows, _cols;

   public:
    /**
     * Constructor.
     *
     * When no arguments are provided, it initializes the object
     * to zero, i.e. rows=, lines=0.
     *
     * @param rows amount of rows
     * @param cols amount of columns.
     */
    Size(int16_t rows = 0, int16_t cols = 0);

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
     * @param cols number of columns
     */
    void cols(int16_t cols);

    /**
     * Set the number of rows.
     *
     * If number of rows is tried to set to a negative value, an
     * out_of_range exception is thrown.
     *
     * @param rows number of rows
     */
    void rows(int16_t rows);

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

inline Size::Size(int16_t rows, int16_t cols) : _rows(rows), _cols(cols) {
    // Please note: The two strings below will not be translated,
    // since that would require including "gettext.h" which is not
    // supposed to be installed.
    if (_rows < 0) throw std::out_of_range("Rows cannot be <0");
    if (_cols < 0) throw std::out_of_range("Columns cannot be <0");
}

inline int16_t Size::cols() const { return _cols; }

inline int16_t Size::rows() const { return _rows; }

inline void Size::cols(int16_t cols) {
    if (cols < 0) throw std::out_of_range("Columns cannot be <0");
    _cols = cols;
}

inline void Size::rows(int16_t rows) {
    if (rows < 0) throw std::out_of_range("Rows cannot be <0");
    _rows = rows;
}

inline const Size& Size::operator+=(const Size& rhs) {
    assert(rhs._cols >= 0);
    assert(rhs._rows >= 0);
    _cols += rhs._cols;
    _rows += rhs._rows;
    return *this;
}

inline const Size& Size::operator-=(const Size& rhs) {
    _cols -= rhs._cols;
    if (_cols < 0) _cols = 0;

    _rows -= rhs._rows;
    if (_rows < 0) _rows = 0;
    return *this;
}

inline bool Size::operator==(const Size& rhs) const {
    return _cols == rhs._cols && _rows == rhs._rows;
}

inline const Size& Size::zero() { return _zero; }

inline Size operator+(const Size& lhs, const Size& rhs) {
    Size tmp = lhs;

    return tmp += rhs;
}

inline Size operator-(const Size& lhs, const Size& rhs) {
    Size tmp = lhs;

    return tmp -= rhs;
}

inline bool operator!=(const Size& lhs, const Size& rhs) {
    return !(lhs == rhs);
}
}  // namespace YACURS

#endif  // SIZE_H
