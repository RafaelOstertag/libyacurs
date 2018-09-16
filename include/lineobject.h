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

#ifndef LINEOBJECT_H
#define LINEOBJECT_H 1

#include <string>
#include "windowbase.h"

namespace YACURS {
/**
 * A line object occupies one entire line of the screen.
 */
class LineObject : public WindowBase {
   public:
    /**
     * Vertical position.
     *
     * Specifies the vertical position of the LineObject.
     */
    enum POSITION {
        /**
         * At the top of the screen.
         */
        POS_TOP,

        /**
         * At the bottom of the screen.
         */
        POS_BOTTOM
    };

    /**
     * Alignment of text.
     */
    enum ALIGNMENT { CENTER, LEFT, RIGHT };

   private:
    std::string _linetext;
    POSITION _position;
    ALIGNMENT _alignment;

    /**
     * Compute and sets the margin of WindowBase to achieve the
     * position of the line object according to @c pos.
     */
    void compute_margin();

    size_t text_length() const;

   protected:
    void put_line();

   public:
    LineObject(POSITION _pos, const std::string& t = std::string(),
               COLOROBJ c = DEFAULT);
    LineObject(const LineObject&) = delete;
    LineObject& operator=(const LineObject&) = delete;
    LineObject(LineObject&&) = delete;
    LineObject& operator=(LineObject&&) = delete;

    virtual ~LineObject();

    void alignment(ALIGNMENT a);

    ALIGNMENT alignment() const;

    void line(const std::string& str);

    std::string line() const;

    // from Realizeable
    void realize();

    void refresh(bool immediate);
};
}  // namespace YACURS

#endif  // LINEOBJECT_H
