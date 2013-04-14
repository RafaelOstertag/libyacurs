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

#ifndef CURSTR_H
#define CURSTR_H

#include <string>

#include "coordinates.h"
#include "colors.h"

namespace YACURS {
	class CurStr : public std::string {
	    private:
		Coordinates __position;
		COLOROBJ __color;

	    public:
		CurStr(const std::string& str,
		       const Coordinates& pos=Coordinates::zero(),
		       COLOROBJ c=DEFAULT);

		CurStr(const std::string& str, int y, int x, COLOROBJ c=DEFAULT);

		CurStr(const CurStr& cs);

		CurStr& operator=(const CurStr& cs);

		void position(const Coordinates& co);
		const Coordinates& position() const;

		void color(COLOROBJ co);
		COLOROBJ color() const;
	};
} // namespace YACURS



#endif // CURSTR_H

