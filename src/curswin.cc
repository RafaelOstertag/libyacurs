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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

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
CursWin::CursWin(WINDOW* win, COLOROBJ dc) : __window(win),
    __def_color(dc),
    __box(false) {
    if (win == 0)
        throw std::invalid_argument(_("argument 'win' must not be 0") );

    int x, y;
    getbegyx(__window, y, x);
    __area.x(x);
    __area.y(y);

    getmaxyx(__window, y, x);
    __area.cols(x);
    __area.rows(y);

    __client_area = __area;

    set_bg(__def_color);
    set_color(__def_color);
}

//
// Public
//
CursWin::CursWin(const Area& _a, COLOROBJ c) : __window(0),
    __def_color(c),
    __box(false),
    __subwin(false) {
    if (_a == Area::zero() ) {
        throw std::invalid_argument(_("Area must not be zero") );
    }

    __area = __client_area = _a;

    if ( (__window = newwin(__area.rows(),
                            __area.cols(),
                            __area.y(),
                            __area.x() ) ) == 0)
        throw EXCEPTIONS::CursesException("newwin");

    set_bg(__def_color);
    set_color(__def_color);
}

CursWin::CursWin(const CursWin& cw) : __window(dupwin(cw.__window) ),
    __def_color(cw.__def_color),
    __box(cw.__box),
    __subwin(cw.__subwin),
    __area(cw.__area),
    __client_area(cw.__client_area) {
    if (__window == 0)
        throw EXCEPTIONS::CursesException("dupwin");
}

CursWin&
CursWin::operator=(const CursWin& cw) {
    if (this == &cw) return *this;

    if (__window != 0) {
        if (delwin(__window) == ERR)
            throw EXCEPTIONS::CursesException("delwin");

        __window = 0;
    }

    __window = cw.__window;
    __def_color = cw.__def_color;

    __box = cw.__box;
    __subwin = cw.__subwin;
    __area = cw.__area;
    __client_area = cw.__client_area;

    return *this;
}

CursWin::~CursWin() {
    if (__window != 0)
        if (delwin(__window) == ERR)
            throw EXCEPTIONS::CursesException("delwin");
}

const YACURS::Area&
CursWin::area() const {
    return __area;
}

const YACURS::Area&
CursWin::client_area() const {
    return __client_area;
}

CursWin&
CursWin::refresh(bool immediate) {
    if (immediate) {
        if (wrefresh(__window) == ERR)
            throw EXCEPTIONS::CursesException("wrefresh");
    } else {
        if (wnoutrefresh(__window) == ERR)
            throw EXCEPTIONS::CursesException("wnoutrefresh");
    }
    return *this;
}

bool
CursWin::issubwin() const {
    return __subwin;
}

bool
CursWin::has_box() const {
    return __box;
}

CursWin&
CursWin::box(chtype verch, chtype horch) {
    if (::box(__window, verch, horch) == ERR)
        throw EXCEPTIONS::CursesException("box");

    __box = true;
    __client_area = __area - Margin(1, 1, 1, 1);

    return *this;
}

CursWin&
CursWin::bkgd(chtype ch) {
    if (wbkgd(__window, ch) == ERR)
        throw EXCEPTIONS::CursesException("wbkgd");

    return *this;
}

CursWin&
CursWin::set_color(COLOROBJ c) {
#if NCURSES_VERSION_PATCH < 20100313
    wattrset(__window, Colors::color_pair(c) );
#else
    if (wattrset(__window, Colors::color_pair(c) ) == ERR)
        throw EXCEPTIONS::CursesException("wattrset");
#endif
    return *this;
}

CursWin&
CursWin::set_bg(COLOROBJ c) {
    wbkgd(__window, ' ' | Colors::color_pair(c) );
    return *this;
}

CursWin&
CursWin::unset_box() {
    __client_area = __area;
    __box = false;

    return *this;
}

YACURS::Coordinates
CursWin::get_cursor() const {
    int y, x;

    getyx(__window, y, x);

    return Coordinates(x, y);
}

CursWin&
CursWin::move(const Coordinates& pos) {
    if (wmove(__window, pos.y(), pos.x() ) == ERR)
        throw EXCEPTIONS::CursesException("wmove");

    return *this;
}

CursWin&
CursWin::clear() {
    if (wclear(__window) == ERR)
        throw EXCEPTIONS::CursesException("wclear");

    return *this;
}

CursWin&
CursWin::erase() {
    if (werase(__window) == ERR)
        throw EXCEPTIONS::CursesException("werase");

    return *this;
}

CursWin&
CursWin::touch() {
    if (touchwin(__window) == ERR)
        throw EXCEPTIONS::CursesException("touchwin");

    return *this;
}

CursWin&
CursWin::untouch() {
    if (untouchwin(__window) == ERR)
        throw EXCEPTIONS::CursesException("untouchwin");

    return *this;
}

bool
CursWin::is_touched() const {
    return is_wintouched(__window) == TRUE;
}

CursWin&
CursWin::addstr(const CurStr& str) {
    set_color(str.color() );

    if (mymvwaddstr(__window,
                    str.position().y(),
                    str.position().x(), str.c_str() ) == ERR &&
        str.position().x() + static_cast<int16_t>(str.length() ) <
        __client_area.cols() )
        throw EXCEPTIONS::CursesException("mvwaddstr");

    set_color(__def_color);
    return *this;
}

CursWin&
CursWin::addstr(const std::string& str) {
    Coordinates coord(get_cursor() );

    if (mywaddstr(__window, str.c_str() ) == ERR &&
        coord.x() + static_cast<int16_t>(str.length() ) <
        __client_area.cols() )
        throw EXCEPTIONS::CursesException("waddstr");

    return *this;
}

CursWin&
CursWin::addstrx(const CurStr& str) {
    // space available to string
    int16_t space = __client_area.cols() - str.position().x() +
                    (__box ? 1 : 0);

    if (space == 1) {
        set_color(str.color() );
        if (__box) {
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
        set_color(__def_color);
        return *this;
    }

    if (space < 1) {
        return *this;
    }

    set_color(str.color() );
    if (space < static_cast<int16_t>(str.length() ) ) {
        addnstr(str, space - 1).addch('>');
    } else {
        addstr(str);
    }
    set_color(__def_color);

    return *this;
}

CursWin&
CursWin::addstrx(const std::string& str) {
    return addstrx(CurStr(str, get_cursor() ) );
}

CursWin&
CursWin::addlinex(const CurStr& str) {
    CurStr tmp(str, Coordinates(__box ? 1 : 0,
                                str.position().y() ), str.color() );

    if (static_cast<int16_t>(tmp.length() + (__box ? 1 : 0) ) <=
        __client_area.cols() ) {
        tmp.append(__client_area.cols() - str.length(), ' ');
        return addstrx(tmp);
    }

    return addstrx(tmp);
}

CursWin&
CursWin::addlinex(const std::string& str) {
    return addlinex(CurStr(str, get_cursor() ) );
}

CursWin&
CursWin::addnstr(const CurStr& str, int n) {
    set_color(str.color() );

    if (mymvwaddnstr(__window,
                     str.position().y(),
                     str.position().x(),
                     str.c_str(), n) == ERR &&
        str.position().x() + n < __client_area.x() )
        throw EXCEPTIONS::CursesException("mvwaddnstr");

    set_color(__def_color);

    return *this;
}

CursWin&
CursWin::addnstr(const std::string& str, int n) {
    Coordinates coord(get_cursor() );

    if (mywaddnstr(__window, str.c_str(), n) == ERR &&
        coord.x() + n < __client_area.x() )
        throw EXCEPTIONS::CursesException("waddnstr");

    return *this;
}

CursWin&
CursWin::addch(const chtype ch) {
    Coordinates end(__area.cols() - 1,
                    __area.rows() - 1);

    if (get_cursor() == end) {
        (void)waddch(__window, ch);
    } else {
        if (waddch(__window, ch) == ERR)
            throw EXCEPTIONS::CursesException("waddch");
    }

    return *this;
}

CursWin&
CursWin::mvaddch(const Coordinates& pos, const chtype ch) {
    Coordinates end(__area.cols() - 1,
                    __area.rows() - 1);

    if (pos == end) {
        (void)mvwaddch(__window, pos.y(), pos.x(), ch);
    } else {
        if (mvwaddch(__window, pos.y(), pos.x(), ch) == ERR)
            throw EXCEPTIONS::CursesException("mvwaddch");
    }
    return *this;
}

CursWin&
CursWin::insch(const chtype ch) {
    if (winsch(__window, ch) == ERR)
        throw EXCEPTIONS::CursesException("winsch");

    return *this;
}

CursWin&
CursWin::mvinsch(const Coordinates& pos, const chtype ch) {
    if (mvwinsch(__window, pos.y(), pos.x(), ch) == ERR)
        throw EXCEPTIONS::CursesException("mvwinsch");

    return *this;
}

CursWin&
CursWin::mvdelch(const Coordinates& pos) {
    if (mvwdelch(__window, pos.y(), pos.x() ) == ERR)
        throw EXCEPTIONS::CursesException("mvwdelch");

    return *this;
}

CursWin&
CursWin::delch() {
    if (wdelch(__window) == ERR)
        throw EXCEPTIONS::CursesException("wdelch");

    return *this;
}

CursWin&
CursWin::clearok(bool fl) {
    if (::clearok(__window, fl ? TRUE : FALSE) == ERR)
        throw EXCEPTIONS::CursesException("clearok");

    return *this;
}

CursWin&
CursWin::scrollok(bool fl) {
    if (::scrollok(__window, fl ? TRUE : FALSE) == ERR)
        throw EXCEPTIONS::CursesException("scrollok");

    return *this;
}

CursWin&
CursWin::leaveok(bool fl) {
    if (::leaveok(__window, fl ? TRUE : FALSE) == ERR)
        throw EXCEPTIONS::CursesException("leaveok");

    return *this;
}

CursWin*
CursWin::derwin(const Area& a) const {
    CursWin* tmp =
        new CursWin(::derwin(__window, a.rows(), a.cols(), a.y(),
                             a.x() ), __def_color);

    tmp->__subwin = true;

    return tmp;
}

CursWin*
CursWin::subwin(const Area& a) const {
    CursWin* tmp =
        new CursWin(::subwin(__window, a.rows(), a.cols(), a.y(),
                             a.x() ), __def_color);

    tmp->__subwin = true;

    return tmp;
}

CursWin&
CursWin::operator+=(const CurStr& str) {
    return addstrx(str);
}

CursWin&
CursWin::operator+=(const std::string& str) {
    return addstrx(str);
}

CursWin&
CursWin::operator<<(const CurStr& str) {
    return addstr(str);
}

CursWin&
CursWin::operator<<(const std::string& str) {
    return addstr(str);
}
