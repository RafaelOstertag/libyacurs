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

#ifndef COLORPARSER_H
#define COLORPARSER_H

#include <vector>
#include <map>

#include "colors.h"
#include "mycurses.h"

namespace YACURS {
    namespace INTERNAL {
        /**
         * Parses Color String.
         *
         * Color        | Abbrev
         * -------------|-------
         * COLOR_BLACK  | k
         * COLOR_RED    | r
         * COLOR_GREEN  | g
         * COLOR_YELLOW | y
         * COLOR_BLUE   | b
         * COLOR_MAGENTA| m
         * COLOR_CYAN   | c
         * COLOR_WHITE  | w
         *
         *
         * Attribute    | Abbrev
         * -------------|-------
         * A_NORMAL     | 0
         * A_STANDOUT   | 1
         * A_UNDERLINE  | 2
         * A_REVERSE    | 3
         * A_BLINK      | 4
         * A_DIM        | 5
         * A_PROTECT    | 6
         * A_INVIS      | 7
         *
         *
         * Objects            | Abbrev
         * -------------------|-------
         * DEFAULT            | DEF
         * DIALOG             | DIA
	 * DIALOG_TITLE       | DIT
         * INPUTWIDGET_NOFOCUS| IWN
         * INPUTWIDGET_FOCUS  | IWF
         * INPUTWIDGET_HIDDEN | IWH
         * BUTTON_NOFOCUS     | BTN
         * BUTTON_FOCUS       | BTF
         * LISTBOX            | LBX
         * LISTBOX_HILITE     | LBH
         * CHECKBOXGROUP      | CBG
         * TITLEBAR           | TLB
         * STATUSBAR          | STB
         *
         * Default String
         * --------------
         *
         * @c "DEF:wk0;DIA:wb0;DIT:bw0;IWN:kw3;IWF:kg3;IWH:yy7;BTN:wk0;BTF:kg3;LBX:wk0;LBH:kg3;CBG:yc0;TLB:bw0;STB:bw0"
         */
        class ColorParser {
            private:
                /**
                 * Default Colors.
                 */
                static const std::string __default_colors;

                std::map<std::string, short> color_name_map;
                std::map<char, short> curs_colors_map;
                std::map<char, int> curs_attrs_map;

                std::vector<std::string> tokenize(const std::string& str)
                const;

                INTERNAL::CursColor process_token(const std::string& tkn);

                std::vector<CursColor> get_default_scheme();

            public:
                ColorParser();
                ColorParser(const ColorParser& cp);
                ColorParser& operator=(const ColorParser& cp);

                std::vector<CursColor> operator()(
                    const std::string& colorstr=default_colors() );

                static const std::string& default_colors();
        };
    } // namespace INTERNAL
} // namespace YACURS

#endif // COLORPARSER_H
