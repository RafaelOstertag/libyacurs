// -*- mode: c++ -*-
//
// This file is part of libyacurs,
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

#ifndef COORDINATES_H
#define COORDINATES_H

#include <stdint.h>
#include <string>

namespace YACURS {
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
            int16_t __y;

            /**
             * x component
             */
            int16_t __x;

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
            Coordinates(int16_t _x=0, int16_t _y=0);

            /**
             * Get x component.
             *
             * @return x component
             */
            int16_t x() const;

            /**
             * Get y component
             *
             * @return y component
             */
            int16_t y() const;

            /**
             * Set x component.
             *
             * @param _x value of x
             */
            void x(int16_t _x);

            /**
             * Set y component.
             *
             * @param _y value of y
             */
            void y(int16_t _y);

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
             * Return the object representing zero.
             *
             * @return Coordinates object representing zero.
             */
            static const Coordinates& zero();
    };

    /**
     * Component wise addition.
     *
     * Adds two Coordinates objects component wise.
     *
     * @param rhs right hand side of addition.
     *
     * @return sum of two Coordinate objects.
     */
    Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs);

    /**
     * Component wise subtraction.
     *
     * Subtracts two Coordinates objects comonent wise.
     *
     * @param rhs right hand side of subtraction.
     *
     * @return difference of two Coordinate objects.
     */
    Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs);

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
    bool operator!=(const Coordinates& lhs, const Coordinates& rhs);
}

#endif // COORDINATES_H
