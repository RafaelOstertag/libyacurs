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

#ifndef WINDOW_H
#define WINDOW_H

#include <map>
#include <string>

#include "mycurses.h"
#include "windowbase.h"
#include "widgetbase.h"
#include "hotkey.h"

namespace YACURS {
    // Forward declaration because widgetbase.h already included window.h
    class WidgetBase;

    /**
     * Window for displaying a widget.
     *
     * @subsection Widget
     *
     * Window is responsible for realizing/unrealizing/refreshing the
     * Widget.
     *
     * A screen resize will result in Window calling resize() of the
     * Widget.
     *
     * Several Widgets can be displayed by the use of Packs.
     *
     * Window will call set
     *
     *  - Curses Window (curses_window())
     *  - Focus Group (focusgroup_id())
     *  - Position (position())
     *  - Available Size (size_available())
     *
     * @subsection Focus
     *
     * Upon realization, Window will create a new Focus Group. When
     * unrealized, it will destroy the Focus Group.
     *
     * Window is responsible for passing along Focus Group ID to
     * Widget.
     *
     * Window will activate its Focus Group upon refresh(). This ensures
     * that the last Window refresh()'ed will have the focus.
     *
     * @subsection Events
     *
     * It implements unrealize(), refresh(), and realize(), which will
     * be called by Event Handlers of WindowBase.
     *
     * In order to implement Hot Keys, it connects to EVT_KEY.
     */
    class Window : public WindowBase {
        private:
            std::map<int, HotKey*> __hot_keys;

            WidgetBase* __widget;

            /**
             * ID of the Focus Group.
             *
             * The ID of the Focus Group belonging to this
             * Window. Supposed to be passed along Widget(s).
             */
            fgid_t __fgid;

            // Not supported
            Window(const Window&);
            Window& operator=(const Window&);

        protected:
            virtual void key_event_handler(Event& _e);

            void unrealize();

        public:
            Window(const Margin& m=Margin() );
            virtual ~Window();

            void widget(WidgetBase* _w);

            WidgetBase* widget() const;

            void add_hotkey(const HotKey& hk);

            void remove_hotkey(const HotKey& hk);

            // Those are from Realizable
            void refresh(bool immediate);

            // Does nothing, everything handled in parent.
            //void resize(const Area& _a);
            void realize();
    };
}

#endif // WINDOW_H
