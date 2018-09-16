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

#ifndef MARGIN_H
#define MARGIN_H 1

namespace YACURS {
class Margin {
   private:
    static Margin __zero;
    int __top, __bottom, __left, __right;

   public:
    Margin(int _top = 0, int _left = 0, int _bottom = 0, int _right = 0);
    Margin(const Margin& m);
    ~Margin();

    void top(int i);

    void bottom(int i);

    void left(int i);

    void right(int i);

    int top() const;

    int bottom() const;

    int left() const;

    int right() const;

    Margin& operator=(const Margin& m);

    Margin operator+(const Margin& m) const;

    Margin operator-(const Margin& m) const;

    bool operator==(const Margin& m) const;

    bool operator!=(const Margin& m) const;

    static const Margin& zero();
};
}  // namespace YACURS

#endif
