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

#ifndef CURSWIN_H
#define CURSWIN_H

#include <list>

#include "colors.h"
#include "area.h"
#include "curstr.h"
#include "mycurses.h"

namespace YACURS {
    /**
     * Name space of internal facilities.
     */
    namespace INTERNAL {
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
                 * Flag whether or not Curses Window has a box.
                 */
                bool __box;

                /**
                 * Flag indicating whether or not this is Subwindow.
                 */
                bool __subwin;

                /**
                 * Size and position of the Window.
                 */
                Area __area;

                /**
                 * Area available to client.
                 */
                Area __client_area;
            protected:
                CursWin(WINDOW* win, COLOROBJ dc);

            public:
                CursWin(const Area& _a, COLOROBJ dc=DEFAULT);
                CursWin(const CursWin& cw);
                CursWin& operator=(const CursWin& cw);

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
                 * Query whether or not Curses Window has been
                 * created.
                 */
                bool created() const;

                /**
                 * Destroy Curses Window.
                 */
                void destroy();

                /**
                 * Refresh Curses Window.
                 *
                 * @param immediate if @c true, call wrefresh() on the Curses
                 * Window, @c false make it call wnoutrefresh().
                 */
                CursWin& refresh(bool immediate=true);

                bool issubwin() const;

                bool has_box() const;

                CursWin& box(chtype verch=0, chtype horch=0);

                CursWin& bkgd(chtype ch);

                CursWin& set_color(COLOROBJ c);

                CursWin& set_bg(COLOROBJ c);

                CursWin& unset_box();

                Coordinates get_cursor() const;

                CursWin& move(const Coordinates& pos);

                CursWin& clear();

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
