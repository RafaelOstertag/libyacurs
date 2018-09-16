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

#ifndef WIDGET_H
#define WIDGET_H 1

#include <cstdlib>
#include <string>

#include "widgetbase.h"
#include "yacurscurses.h"

namespace YACURS {
/**
 * Implements a Widget.
 *
 * @subsection Basics
 *
 * Upon realize(), only the Curses Window will be created. refresh()
 * only calls wrefresh() or wnoutrefresh(). unrealize() will destroy
 * the Curses Window.
 *
 * @subsection Content
 *
 * Content in Curses Window should only be drawn upon refresh().
 */
class Widget : public WidgetBase {
   private:
    /**
     * curses subwin used by widgets
     */
    YACURS::INTERNAL::CursWin* __widget_subwin;

    // Not supported
    Widget& operator=(const Widget&);

   protected:
    void redraw_handler(Event& _e);

    void force_refresh_handler(Event& _e);

    void unrealize();

    YACURS::INTERNAL::CursWin* widget_subwin() const;

   public:
    Widget();

    virtual ~Widget();

    // Must be overriden in derived classes
    // bool size_change();

    // Inherited from Realizeable
    void refresh(bool immediate);

    void resize(const Area& _a);

    /**
     * Realize the widget.
     *
     * realizes the widget by setting up a curses subwindow, which
     * can be used by derived classes.
     */
    void realize();
};
}  // namespace YACURS
#endif  // WIDGET_H
