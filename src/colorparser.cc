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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "gettext.h"

#include <stdexcept>
#include <vector>

#include "colorparser.h"

using namespace YACURS::INTERNAL;

//
// Private
//
const std::string ColorParser::__default_colors(
    "DEF:wk0;DIA:wb0;DIT:bw0;IWN:kw3;IWF:kg3;IWH:yy7;BTN:wk0;BTF:kg3;LBX:wk0;LBH:kg3;CBG:yc0;TLB:bw0;STB:bw0");

std::vector<std::string>
ColorParser::tokenize(const std::string& str) const {
    std::string::size_type pos;
    std::vector<std::string> retval(0);

    std::string tmpstr(str);

    while ( (pos = tmpstr.find(';') ) != std::string::npos) {
        retval.push_back(tmpstr.substr(0, pos) );
        tmpstr.erase(0, pos + 1);
    }

    // Sort in remaining parts
    if (tmpstr.length() == 7)
        retval.push_back(tmpstr);

    return retval;
}

CursColor
ColorParser::process_token(const std::string& tkn) {
    std::string::size_type pos = tkn.find(':');

    if (pos == std::string::npos) {
        std::string tmp = "'" + tkn + _(" does not contain ':'");
        throw std::invalid_argument(tmp);
    }

    std::string col_obj = tkn.substr(0, pos);
    if (col_obj.length() != 3) {
        std::string tmp = _("Color Object size invalid");
        throw std::invalid_argument(tmp);
    }

    std::string cols = tkn.substr(pos + 1, tkn.length() - pos - 1);
    if (cols.length() != 3) {
        std::string tmp = _("Color size invalid");
        throw std::invalid_argument(tmp);
    }

    CursColor retval;
    if (color_name_map.find(col_obj) == color_name_map.end() ) {
        col_obj += _(" is not valid color object");
        throw std::out_of_range(col_obj);
    }

    retval.no = color_name_map[col_obj];

    if (curs_colors_map.find(cols[0]) == curs_colors_map.end() ) {
        std::string tmp = _("not a valid color: ");
        tmp += cols[0];
        throw std::out_of_range(tmp);
    }
    retval.fg = curs_colors_map[cols[0]];

    if (curs_colors_map.find(cols[1]) == curs_colors_map.end() ) {
        std::string tmp = _("not a valid color: ");
        tmp += cols[1];
        throw std::out_of_range(tmp);
    }
    retval.bg = curs_colors_map[cols[1]];

    if (curs_attrs_map.find(cols[2]) == curs_attrs_map.end() ) {
        std::string tmp = _("not a valid attribute: ");
        tmp += cols[2];
        throw std::out_of_range(tmp);
    }
    retval.attr = curs_attrs_map[cols[2]];

    return retval;
}

std::vector<CursColor>
ColorParser::get_default_scheme() {
    std::vector<std::string> default_tokens = tokenize(default_colors() );

    std::vector<CursColor> retval(NUMBER_OF_COLOROBJ);

    std::vector<std::string>::iterator it = default_tokens.begin();

    while (it != default_tokens.end() ) {
        // We don't catch exceptions, since there must be none. The
        // default color scheme has to be flawless.
        CursColor tmp = process_token(*it++);
        // The color numbers are off by +1 due to NCurses
        retval.at(tmp.no - 1) = tmp;
    }

    return retval;
}

//
// Protected
//

//
// Public
//
ColorParser::ColorParser() {
    // The numerical values of the color name has an offset of 1,
    // because NCurses expects the color pair numbering to start with
    // 1. X/Open Curses allows the numbering to start with 0, though.
    color_name_map["DEF"] = DEFAULT + 1;
    color_name_map["DIA"] = DIALOG + 1;
    color_name_map["DIT"] = DIALOG_TITLE + 1;
    color_name_map["IWN"] = INPUTWIDGET_NOFOCUS + 1;
    color_name_map["IWF"] = INPUTWIDGET_FOCUS + 1;
    color_name_map["IWH"] = INPUTWIDGET_HIDDEN + 1;
    color_name_map["BTN"] = BUTTON_NOFOCUS + 1;
    color_name_map["BTF"] = BUTTON_FOCUS + 1;
    color_name_map["LBX"] = LISTBOX + 1;
    color_name_map["LBH"] = LISTBOX_HILITE + 1;
    color_name_map["CBG"] = CHECKBOXGROUP + 1;
    color_name_map["TLB"] = TITLEBAR + 1;
    color_name_map["STB"] = STATUSBAR + 1;

    curs_colors_map['k'] = COLOR_BLACK;
    curs_colors_map['r'] = COLOR_RED;
    curs_colors_map['g'] = COLOR_GREEN;
    curs_colors_map['y'] = COLOR_YELLOW;
    curs_colors_map['b'] = COLOR_BLUE;
    curs_colors_map['m'] = COLOR_MAGENTA;
    curs_colors_map['c'] = COLOR_CYAN;
    curs_colors_map['w'] = COLOR_WHITE;

    curs_attrs_map['0'] = A_NORMAL;
    curs_attrs_map['1'] = A_STANDOUT;
    curs_attrs_map['2'] = A_UNDERLINE;
    curs_attrs_map['3'] = A_REVERSE;
    curs_attrs_map['4'] = A_BLINK;
    curs_attrs_map['5'] = A_DIM;
    curs_attrs_map['6'] = A_PROTECT;
    curs_attrs_map['7'] = A_INVIS;
}

ColorParser::ColorParser(const ColorParser& cp) : color_name_map(
        cp.color_name_map),
    curs_colors_map(cp.curs_colors_map),
    curs_attrs_map(cp.curs_attrs_map) {
}

ColorParser&
ColorParser::operator=(const ColorParser& cp) {
    if (this == &cp) return *this;

    color_name_map = cp.color_name_map;
    curs_colors_map = cp.curs_colors_map;
    curs_attrs_map = cp.curs_attrs_map;

    return *this;
}

std::vector<CursColor>
ColorParser::operator()(const std::string& colorstr) {
    std::vector<CursColor> default_scheme = get_default_scheme();

    std::vector<std::string> tokens = tokenize(colorstr);

    std::vector<CursColor> retval(NUMBER_OF_COLOROBJ);

    if (tokens.size() > 0) {
        std::vector<std::string>::iterator it = tokens.begin();
        while (it != tokens.end() ) {
            try {
                CursColor tmp = process_token(*it);
                // Color number is off by +1 due to NCurses
                retval.at(tmp.no - 1) = tmp;
            } catch (std::out_of_range&) {
                // ignore
            }
            it++;
        }

        // Merge with default colors, so unspecified colors will be
        // initialized.
        for (int i = 0; i < NUMBER_OF_COLOROBJ; i++) {
            if (retval.at(i).no == -1)
                retval.at(i) = default_scheme.at(i);
        }
    } else {
        return default_scheme;
    }

    return retval;
}

const std::string&
ColorParser::default_colors() {
    return __default_colors;
}
