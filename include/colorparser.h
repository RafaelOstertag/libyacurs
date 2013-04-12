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

#ifndef COLORPARSER_H
#define COLORPARSER_H

#include <vector>
#include <map>

#include "colors.h"
#include "mycurses.h"

namespace YACURS {
    class ColorParser {
	private:
	    static const std::string __default_colors;

	    std::map<std::string,short> color_name_map;
	    std::map<char,short> curs_colors_map;
	    std::map<char,int> curs_attrs_map;

	    std::vector<std::string> tokenize(const std::string& str) const;
	    CursColor process_token(const std::string& tkn);
	    std::vector<CursColor> get_default_scheme();

	public:
	    ColorParser();
	    ColorParser(const ColorParser& cp);
	    ColorParser& operator=(const ColorParser& cp);
	    std::vector<CursColor> operator()(const std::string& colorstr=default_colors());

	    static const std::string& default_colors();
    };
}

#endif // COLORPARSER_H
