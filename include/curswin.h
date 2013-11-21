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

#ifndef CURSWIN_H
#define CURSWIN_H 1

#include <list>

#include "colors.h"
#include "area.h"
#include "curstr.h"
#include "yacurscurses.h"

namespace YACURS {
    /**
     * Name space of internal facilities.
     */
    namespace INTERNAL {
        /**
         * Wrapper class for WINDOW.
         *
         * CursWin provides wrapper for Curses WINDOW and
         * corresponding member functions to modfiy WINDOW.
         *
         * Member functions are named like their Curses counterparts,
         * except for the @c w prefix, e.g. @c wclear() becomes @c
         * clear().
         *
         * Member functions writing character strings in CursWin come
         * in two flavors: the ones taking @c std::string as argument,
         * and the other taking @c YACURS::CursStr.
         *
         * CursWin features a default color, which
         */
        class CursWin {
            private:
                /**
                 * Pointer to Curses Window.
                 */
                WINDOW* __window;

                /**
                 * Default color for Curses Window.
                 */
                COLOROBJ __def_color;

                /**
                 * Size and position of the Window.
                 */
                Area __area;

                /**
                 * Area available to client.
                 */
                Area __client_area;

                /**
                 * Flag whether or not Curses Window has a box.
                 */
                bool __box;

                /**
                 * Flag indicating whether or not this is Subwindow.
                 */
                bool __subwin;

		/**
		 * Horizontal character for box.
		 */
		chtype __horch;

		/**
		 * Vertical character for box.
		 */
		chtype __verch;
            protected:
                /**
                 * Initialize CursWin from WINDOW.
                 *
                 * @param win pointer to Curses WINDOW.
                 *
                 * @param dc default color.
                 */
                CursWin(WINDOW* win, COLOROBJ dc, bool subwin);

            public:
                /**
                 * Constructor.
                 *
                 * @param _a area the occupied on screen
                 *
                 * @param dc default color
                 */
                CursWin(const Area& _a, COLOROBJ dc=DEFAULT);

                /**
                 * Copy Constructor.
                 */
                CursWin(const CursWin& cw);

                /**
                 * Assignment operator.
                 */
                CursWin& operator=(const CursWin& cw);

                /**
                 * Destructor.
                 */
                ~CursWin();

                /**
                 * Size and position of the Window.
                 *
                 * @return size and position of the Window.
                 */
                const Area& area() const;

                /**
                 * Size and position available to clients.
                 *
                 * Size and position available for use.
                 *
                 * @return size and position.
                 */
                const Area& client_area() const;

                /**
                 * Refresh Curses Window.
                 *
                 * @param immediate if @c true, call wrefresh() on the
                 * Curses Window, @c false make it call
                 * wnoutrefresh().
                 *
                 * @return reference to *this.
                 */
                CursWin& refresh(bool immediate=true);

                /**
                 * Get sub window flag.
                 *
                 * @return @c true if CursWin is a sub window, @c
                 * false otherwise.
                 */
                bool issubwin() const;

                /**
                 * Get box flag.
                 *
                 * @return @c true if CursWin has box, @c false
                 * otherwise.
                 */
                bool has_box() const;

                /**
                 * Create box.
                 *
                 * Draw a box around the edges of a window
                 *
                 * @param verch vertical character. If zero, default
                 * value is used.
                 *
                 * @param horch horizontal character. If zero, default
                 * value is used.
                 *
                 * @return reference to *this.
                 *
                 * @internal wrapper for box().
                 */
                CursWin& box(chtype verch=0, chtype horch=0);

                /**
                 * Set background property.
                 *
                 * Set the background property of the current or
                 * specified window and then apply this setting to
                 * every character position in that window
                 *
                 * @param ch rendition.
                 *
                 * @return reference to *this.
                 *
                 * @internal wrapper for bgkd().
                 */
                CursWin& bkgd(chtype ch);

                /**
                 * Set Color.
                 *
                 * @param c color.
                 *
                 * @sa COLOROBJ
                 */
                CursWin& set_color(COLOROBJ c);

                CursWin& set_bg(COLOROBJ c);

                CursWin& unset_box();

                Coordinates get_cursor() const;

                CursWin& move(const Coordinates& pos);

                CursWin& clear();

                CursWin& clrtobot();

                CursWin& erase();

                CursWin& touch();

                CursWin& untouch();

                bool is_touched() const;

                CursWin& addstr(const CurStr& str);

                CursWin& addstr(const std::string& str);

                CursWin& addstrx(const CurStr& str);

                CursWin& addstrx(const std::string& str);

                CursWin& addlinex(const CurStr& str);

                CursWin& addlinex(const std::string& str);

                CursWin& addnstr(const CurStr& str, int n);

                CursWin& addnstr(const std::string& str, int n);

                CursWin& addch(const chtype ch);

                CursWin& mvaddch(const Coordinates& pos, const chtype ch);

                CursWin& insch(const chtype ch);

                CursWin& mvinsch(const Coordinates& pos, const chtype ch);

                CursWin& mvdelch(const Coordinates& pos);

                CursWin& delch();

		/**
		 * Horizontal rule
		 *
		 * @param ypos only the y-component is used.
		 */
		CursWin& hrule(const Coordinates& ypos=Coordinates::zero());

		/**
		 * Vertical rule
		 *
		 * @param xpos only the x-component is used.
		 */
		CursWin& vrule(const Coordinates& xpos=Coordinates::zero());

                CursWin& clearok(bool fl);

                CursWin& scrollok(bool fl);

                CursWin& leaveok(bool fl);

                CursWin* derwin(const Area& a) const;

                CursWin* subwin(const Area& a) const;

                CursWin& operator+=(const CurStr& str);

                CursWin& operator+=(const std::string& str);

                CursWin& operator<<(const CurStr& str);

                CursWin& operator<<(const std::string& str);
        };
    } // namespace INTERNAL
} // namespace YACURS

#endif // CURSWIN_H
