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

#include <cerrno>
#include <cstdlib>

#include "colors.h"
#include "curs.h"
#include "eventqueue.h"
#include "lineobject.h"
#include "yacursconst.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

void LineObject::compute_margin() {
#ifndef NDEBUG
    Margin _m_debug;
#endif  // NDEBUG
    switch (_position) {
        case POS_TOP:
#ifdef NDEBUG
            margin(Margin(0, 0, area().rows() - 1, 0));
#else   // NDEBUG
            _m_debug = Margin(0, 0, area().rows() - 1, 0);
            assert(_m_debug.bottom() >= 0);
            margin(_m_debug);
#endif  // NDEBUG
            break;

        case POS_BOTTOM:
#ifdef NDEBUG
            margin(Margin(area().rows() - 1, 0, 0, 0));
#else  // NDEBUG
            _m_debug = Margin(area().rows() - 1, 0, 0, 0);
            assert(_m_debug.top() >= 0);
            margin(_m_debug);
#endif
            break;
    }
}

size_t LineObject::text_length() const {
#ifdef YACURS_USE_WCHAR
    size_t mbslen = std::mbstowcs(0, _linetext.c_str(), 0);
    if (mbslen == (size_t)-1) throw EXCEPTIONS::SystemError(errno);

    return mbslen;
#else
    return _linetext.length();
#endif
}

//
// Protected
//

void LineObject::put_line() {
    if (!(realization() == REALIZED || realization() == REALIZING)) return;

    curses_window()->erase();

    if (text_length() < 1) return;

    assert(area().cols() >= MIN_COLS);
    if (static_cast<std::string::size_type>(area().cols()) <= text_length()) {
        // Since we are here, the text is too big for the screen
        // width, so we can't align anyway.
        CurStr tmp(_linetext, Coordinates(0, 0));
        curses_window()->addstrx(tmp);
    } else {
        int hpos = 0;
        switch (_alignment) {
            case LEFT:
                // Nothing to do, since hpos is == 0
                assert(hpos == 0);
                break;

            case CENTER:
                assert(static_cast<std::string::size_type>(area().cols()) >=
                       text_length());
                hpos = (area().cols() - text_length()) / 2;
                break;

            case RIGHT:
                assert(static_cast<std::string::size_type>(area().cols()) >=
                       text_length());
                hpos = area().cols() - text_length();
                break;
        }

        CurStr tmp(_linetext, Coordinates(hpos, 0), color());
        curses_window()->addstr(tmp);
    }
}

//
// Public
//
LineObject::LineObject(POSITION pos, const std::string& t, COLOROBJ c)
    : _linetext(t), _position(pos), _alignment(LEFT) {
    color(c);
}

LineObject::~LineObject() {}

void LineObject::line(const std::string& str) {
    _linetext = str;
    // Refresh is responsible for taking care of whether or not the
    // refresh can happen, for instance, it cannot happen if the
    // object is not realized.
    refresh(true);
}

void LineObject::alignment(ALIGNMENT a) {
    _alignment = a;

    if (realization() == REALIZED) refresh(true);
}

LineObject::ALIGNMENT LineObject::alignment() const { return _alignment; }

std::string LineObject::line() const { return _linetext; }

void LineObject::realize() {
    REALIZE_ENTER;
    compute_margin();
    WindowBase::realize();
    REALIZE_LEAVE;
}

void LineObject::refresh(bool immediate) {
    if (realization() != REALIZED) return;
    put_line();

    curses_window()->set_color(YACURS::DEFAULT);

    WindowBase::refresh(immediate);
}
