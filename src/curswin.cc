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
#include "config.h"
#endif

#include "gettext.h"

#include <cerrno>
#include <cstdlib>
#include <stdexcept>

#include "curswin.h"
#include "yacursex.h"

using namespace YACURS::INTERNAL;
//
// Private
//

//
// Protected
//
CursWin::CursWin(WINDOW* win, COLOROBJ dc, bool subwin)
    : _window{win},
      _def_color{dc},
      _box{false},
      _subwin{subwin},
      _horch{0},
      _verch{0} {
    if (win == 0)
        throw std::invalid_argument(_("argument 'win' must not be 0"));

    int x, y;
    getbegyx(_window, y, x);
    _area.x(x);
    _area.y(y);

    getmaxyx(_window, y, x);
    _area.cols(x);
    _area.rows(y);

    _client_area = _area;

    set_bg(_def_color);
    set_color(_def_color);
}

//
// Public
//
CursWin::CursWin(const Area& a, COLOROBJ c)
    : _window{0},
      _def_color{c},
      _box{false},
      _subwin{false},
      _horch{0},
      _verch{0} {
    if (a == Area::zero()) {
        throw std::invalid_argument(_("Area must not be zero"));
    }

    _area = _client_area = a;

    if ((_window = newwin(_area.rows(), _area.cols(), _area.y(), _area.x())) ==
        0)
        throw EXCEPTIONS::CursesException("newwin");

    set_bg(_def_color);
    set_color(_def_color);
}

CursWin::CursWin(const CursWin& cw)
    : _window{dupwin(cw._window)},
      _def_color{cw._def_color},
      _area{cw._area},
      _client_area{cw._client_area},
      _box{cw._box},
      _subwin{cw._subwin},
      _horch{cw._horch},
      _verch{cw._verch} {
    if (_window == 0) throw EXCEPTIONS::CursesException("dupwin");
}

CursWin::CursWin(CursWin&& cw)
    : _window(cw._window),
      _def_color(cw._def_color),
      _area(std::move(cw._area)),
      _client_area(std::move(cw._client_area)),
      _box(cw._box),
      _subwin(cw._subwin),
      _horch(cw._horch),
      _verch(cw._verch) {
    cw._window = nullptr;
}

CursWin& CursWin::operator=(const CursWin& cw) {
    if (this == &cw) return *this;

    if (_window != 0) {
        if (delwin(_window) == ERR) throw EXCEPTIONS::CursesException("delwin");

        _window = 0;
    }

    _window = cw._window;
    _def_color = cw._def_color;

    _box = cw._box;
    _subwin = cw._subwin;
    _area = cw._area;
    _client_area = cw._client_area;

    _horch = cw._horch;
    _verch = cw._verch;

    return *this;
}

CursWin& CursWin::operator=(CursWin&& cw) {
    if (this == &cw) return *this;

    _window = cw._window;
    cw._window = nullptr;
    _def_color = cw._def_color;

    _box = cw._box;
    _subwin = cw._subwin;
    _area = std::move(cw._area);
    _client_area = std::move(cw._client_area);

    _horch = cw._horch;
    _verch = cw._verch;

    return *this;
}

CursWin::~CursWin() {
    if (_window != 0) delwin(_window);
}

const YACURS::Area& CursWin::area() const { return _area; }

const YACURS::Area& CursWin::client_area() const { return _client_area; }

CursWin& CursWin::refresh(bool immediate) {
    if (immediate) {
        if (wrefresh(_window) == ERR)
            throw EXCEPTIONS::CursesException("wrefresh");
    } else {
        if (wnoutrefresh(_window) == ERR)
            throw EXCEPTIONS::CursesException("wnoutrefresh");
    }
    return *this;
}

bool CursWin::issubwin() const { return _subwin; }

bool CursWin::has_box() const { return _box; }

CursWin& CursWin::box(chtype verch, chtype horch) {
    _verch = verch;
    _horch = horch;
    // X/Open has trouble with colors and ACS_* characters, as used
    // with borders. Therefore, we fall back to non ACS_*
    // characters. Beware, NCurses also defines _XOPEN_CURSES,
    // though.
#if !defined(_XOPEN_CURSES) || defined(NCURSES_VERSION)
    if (::box(_window, _verch, _horch) == ERR)
        throw EXCEPTIONS::CursesException("box");
#else
    if (wborder(_window, _verch == 0 ? '|' : _verch, _verch == 0 ? '|' : _verch,
                _horch == 0 ? '-' : _horch, _horch == 0 ? '-' : _horch, '+',
                '+', '+', '+') == ERR)
        throw EXCEPTIONS::CursesException("wborder");
#endif

    _box = true;
    _client_area = _area - Margin(1, 1, 1, 1);

    return *this;
}

CursWin& CursWin::bkgd(chtype ch) {
    if (wbkgd(_window, ch) == ERR) throw EXCEPTIONS::CursesException("wbkgd");

    return *this;
}

CursWin& CursWin::set_color(COLOROBJ c) {
#if NCURSES_VERSION_PATCH < 20100313
    wattrset(_window, Colors::color_pair(c));
#else
    if (wattrset(_window, Colors::color_pair(c)) == ERR)
        throw EXCEPTIONS::CursesException("wattrset");
#endif
    return *this;
}

CursWin& CursWin::set_bg(COLOROBJ c) {
    bkgd(' ' | Colors::color_pair(c));
    return *this;
}

CursWin& CursWin::unset_box() {
    _client_area = _area;
    _box = false;

    return *this;
}

YACURS::Coordinates CursWin::get_cursor() const {
    int y, x;

    getyx(_window, y, x);

    return Coordinates(x, y);
}

CursWin& CursWin::move(const Coordinates& pos) {
    if (wmove(_window, pos.y(), pos.x()) == ERR)
        throw EXCEPTIONS::CursesException("wmove");

    return *this;
}

CursWin& CursWin::clear() {
    if (wclear(_window) == ERR) throw EXCEPTIONS::CursesException("wclear");

    // This has been introduced with the EVT_REDRAW event.
    //
    // After a erase, the box is gone, so we assume that when we have
    // a box, it should stay.
    if (_box) box(_verch, _horch);

    return *this;
}

CursWin& CursWin::clrtobot() {
    if (wclrtobot(_window) == ERR)
        throw EXCEPTIONS::CursesException("wclrtobot");

    return *this;
}

CursWin& CursWin::erase() {
    if (werase(_window) == ERR) throw EXCEPTIONS::CursesException("werase");

    // After a erase, the box is gone, so we assume that when we have
    // a box, it should stay.
    if (_box) box(_verch, _horch);

    return *this;
}

CursWin& CursWin::touch() {
    if (touchwin(_window) == ERR) throw EXCEPTIONS::CursesException("touchwin");

    return *this;
}

CursWin& CursWin::untouch() {
    if (untouchwin(_window) == ERR)
        throw EXCEPTIONS::CursesException("untouchwin");

    return *this;
}

bool CursWin::is_touched() const { return is_wintouched(_window) == TRUE; }

CursWin& CursWin::addstr(const CurStr& str) { return addnstr(str, -1); }

CursWin& CursWin::addstr(const std::string& str) {
    return addstr(CurStr(str, get_cursor()));
}

CursWin& CursWin::addstrx(const CurStr& str) {
    // space available to string
    int16_t space = _client_area.cols() - str.position().x() + (_box ? 1 : 0);

    if (space == 1) {
        set_color(str.color());
        if (_box) {
            // mvaddch advances cursor, but does not move characters
            // under cursor, which is ideal when placing character
            // rigtht next to the border.
            mvaddch(str.position(), '>');
        } else {
            // does not advance cursor, but moves everything under it
            // to the right, which is ideal when having no box. If
            // insch() would be used with box, the vert box char right
            // next to the cursor is moved off the window.
            mvinsch(str.position(), '>');
        }
        set_color(_def_color);
        return *this;
    }

    if (space < 1) {
        return *this;
    }

    set_color(str.color());
#ifdef YACURS_USE_WCHAR
    // addnstr() will convert to wide char, we're only interested in
    // length here.
    size_t _mbslen = std::mbstowcs((wchar_t*)0, str.c_str(), 1);
    if (_mbslen == (size_t)-1) throw EXCEPTIONS::SystemError(errno);

    if (space < static_cast<int16_t>(_mbslen)) {
        addnstr(str, space - 1).addch('>');
    } else {
        addstr(str);
    }
#else
    if (space < static_cast<int16_t>(str.length())) {
        addnstr(str, space - 1).addch('>');
    } else {
        addstr(str);
    }
#endif
    set_color(_def_color);

    return *this;
}

CursWin& CursWin::addstrx(const std::string& str) {
    return addstrx(CurStr(str, get_cursor()));
}

CursWin& CursWin::addlinex(const CurStr& str) {
    CurStr tmp(str, Coordinates(_box ? 1 : 0, str.position().y()), str.color());

    size_t mylen;

#ifdef YACURS_USE_WCHAR
    mylen = std::mbstowcs((wchar_t*)0, str.c_str(), 0);
    if (mylen == (size_t)-1) throw EXCEPTIONS::SystemError(errno);
#else
    mylen = tmp.length();
#endif

    if (static_cast<int16_t>(mylen + (_box ? 1 : 0)) <= _client_area.cols()) {
        tmp.append(_client_area.cols() - mylen, ' ');
        return addstrx(tmp);
    }

    return addstrx(tmp);
}

CursWin& CursWin::addlinex(const std::string& str) {
    return addlinex(CurStr(str, get_cursor()));
}

CursWin& CursWin::addnstr(const CurStr& str, int n) {
    set_color(str.color());

#ifdef YACURS_USE_WCHAR
    size_t buflen = std::mbstowcs((wchar_t*)0, str.c_str(), 0);
    if (buflen == (size_t)-1) throw EXCEPTIONS::SystemError(errno);

    wchar_t* wbuffer = new wchar_t[buflen + 1];

    size_t retval = std::mbstowcs(wbuffer, str.c_str(), buflen + 1);
    assert(buflen == retval);
    if (retval == (size_t)-1) {
        delete[] wbuffer;
        throw EXCEPTIONS::SystemError(errno);
    }
    wbuffer[retval] = L'\0';

    if (mvwaddnwstr(_window, str.position().y(), str.position().x(), wbuffer,
                    n) == ERR &&
        (str.position().x() + (n == -1 ? static_cast<int16_t>(retval) : n)) <
            _client_area.cols()) {
        delete[] wbuffer;
        throw EXCEPTIONS::CursesException("mvwaddnwstr");
    }

    delete[] wbuffer;
#else
    if (mymvwaddnstr(_window, str.position().y(), str.position().x(),
                     str.c_str(), n) == ERR &&
        (str.position().x() + (n == -1 ? static_cast<int16_t>(str.length())
                                       : n)) < _client_area.cols())
        throw EXCEPTIONS::CursesException("mvwaddnstr");
#endif

    set_color(_def_color);

    return *this;
}

CursWin& CursWin::addnstr(const std::string& str, int n) {
    return addnstr(CurStr(str, get_cursor()), n);
}

CursWin& CursWin::addch(const chtype ch) {
    Coordinates end(_area.cols() - 1, _area.rows() - 1);

    if (get_cursor() == end) {
        (void)waddch(_window, ch);
    } else {
        if (waddch(_window, ch) == ERR)
            throw EXCEPTIONS::CursesException("waddch");
    }

    return *this;
}

CursWin& CursWin::mvaddch(const Coordinates& pos, const chtype ch) {
    Coordinates end(_area.cols() - 1, _area.rows() - 1);

    if (pos == end) {
        (void)mvwaddch(_window, pos.y(), pos.x(), ch);
    } else {
        if (mvwaddch(_window, pos.y(), pos.x(), ch) == ERR)
            throw EXCEPTIONS::CursesException("mvwaddch");
    }
    return *this;
}

CursWin& CursWin::insch(const chtype ch) {
    if (winsch(_window, ch) == ERR) throw EXCEPTIONS::CursesException("winsch");

    return *this;
}

CursWin& CursWin::mvinsch(const Coordinates& pos, const chtype ch) {
    if (mvwinsch(_window, pos.y(), pos.x(), ch) == ERR)
        throw EXCEPTIONS::CursesException("mvwinsch");

    return *this;
}

CursWin& CursWin::mvdelch(const Coordinates& pos) {
    if (mvwdelch(_window, pos.y(), pos.x()) == ERR)
        throw EXCEPTIONS::CursesException("mvwdelch");

    return *this;
}

CursWin& CursWin::delch() {
    if (wdelch(_window) == ERR) throw EXCEPTIONS::CursesException("wdelch");

    return *this;
}

CursWin& CursWin::hrule(const Coordinates& ypos) {
    Coordinates where(ypos);

    where.x(_box ? 1 : 0);
    if (ypos == Coordinates::zero()) {
        where.y(_box ? 1 : 0);
    }

#if !defined(_XOPEN_CURSES) || defined(NCURSES_VERSION)
    if (mvwhline(_window, where.y(), where.x(), 0, _client_area.cols()) == ERR)
        throw EXCEPTIONS::CursesException("mvwhline");
#else
    if (mvwhline(_window, where.y(), where.x(), '_', _client_area.cols()) ==
        ERR)
        throw EXCEPTIONS::CursesException("mvwhline");
#endif

    return *this;
}

CursWin& CursWin::vrule(const Coordinates& xpos) {
    Coordinates where(xpos);

    where.y(_box ? 1 : 0);
    if (xpos == Coordinates::zero()) {
        where.x(_box ? 1 : 0);
    }

#if !defined(_XOPEN_CURSES) || defined(NCURSES_VERSION)
    if (mvwvline(_window, where.y(), where.x(), 0, _client_area.rows()) == ERR)
        throw EXCEPTIONS::CursesException("mvwvline");
#else
    if (mvwvline(_window, where.y(), where.x(), '|', _client_area.rows()) ==
        ERR)
        throw EXCEPTIONS::CursesException("mvwvline");
#endif

    return *this;
}

CursWin& CursWin::clearok(bool fl) {
    if (::clearok(_window, fl ? TRUE : FALSE) == ERR)
        throw EXCEPTIONS::CursesException("clearok");

    return *this;
}

CursWin& CursWin::scrollok(bool fl) {
    if (::scrollok(_window, fl ? TRUE : FALSE) == ERR)
        throw EXCEPTIONS::CursesException("scrollok");

    return *this;
}

CursWin& CursWin::leaveok(bool fl) {
    if (::leaveok(_window, fl ? TRUE : FALSE) == ERR)
        throw EXCEPTIONS::CursesException("leaveok");

    return *this;
}

CursWin* CursWin::derwin(const Area& a) const {
    WINDOW* _subwin = ::derwin(_window, a.rows(), a.cols(), a.y(), a.x());
    if (_subwin == 0) throw EXCEPTIONS::CursesException("derwin");

    return new CursWin(_subwin, _def_color, true);
}

CursWin* CursWin::subwin(const Area& a) const {
    WINDOW* _subwin = ::subwin(_window, a.rows(), a.cols(), a.y(), a.x());
    if (_subwin == 0) throw EXCEPTIONS::CursesException("subwin");

    return new CursWin(_subwin, _def_color, true);
}

CursWin& CursWin::operator+=(const CurStr& str) { return addstrx(str); }

CursWin& CursWin::operator+=(const std::string& str) { return addstrx(str); }

CursWin& CursWin::operator<<(const CurStr& str) { return addstr(str); }

CursWin& CursWin::operator<<(const std::string& str) { return addstr(str); }
