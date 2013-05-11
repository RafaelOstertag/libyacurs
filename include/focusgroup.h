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

#ifndef FOCUSGROUP_H
#define FOCUSGROUP_H

#include <list>

namespace YACURS {
    class WidgetBase;

    /**
     * @ingroup Focus
     *
     * Focus Group
     *
     * A Focus Group is comprised of Widgets with one Widget having the
     * focus, if the Group is active (or the Current Focus Group with
     * regards to the Focus Manager).
     *
     * Widgets should emit @c EVT_FOCUS_NEXT or @c EVT_FOCUS_PREVIOUS if
     * they voluntarely give up focus. The focus will then go to the next
     * (or previous) Widget in the Focus Group, by calling
     * WidgetBase::focus().
     *
     * The FocusManager is intended to be the only interface to
     * FocusGroups for other objects.
     *
     * @sa FocusManager
     */
    class FocusGroup {
        private:
            /**
             * Indicates the state of the Focus Group.
             *
             * @c true means the Focus Group is active, @c false it is
             * inactive.
             */
            bool __active;

            /**
             * All the widgets belonging to the Focus Group.
             */
            std::list<WidgetBase*> __widgets;

            /**
             * The Widget having the focus in the group if active.
             */
            std::list<WidgetBase*>::iterator __focus;

        public:
            /**
             * Constructor.
             *
             * Upon construction, FocusGroup is not active. In order to
             * activate the FocusGroup, call FocusGroup::activate.
             */
            FocusGroup();

            /**
             * Copy Constructor.
             *
             * @parma _f reference to FocusGroup.
             */
            FocusGroup(const FocusGroup& _f);
            ~FocusGroup();
            FocusGroup& operator=(const FocusGroup& _f);

            /**
             * Activate Focus Group
             *
             * When activating a FocusGroup, Widgets won't be focused by
             * calling focus(). An explicit call to FocusGroup::refocus()
             * is necessary. This is in order to prevent refresh issues
             * with overlapping Windows during a screen resize.
             */
            void activate();

            /**
             * Deactivate Focus Group.
             *
             * The Widget having the Focus will loose it, and any calls to
             * focus_next() or focus_previous() will be ignored.
             */
            void deactivate();

            /**
             * Query whether or not the Focus Group is active.
             *
             * @return @c true if the Focus Group is active, @c false
             * otherwise.
             */
            bool active() const;

            /**
             * Add Widget to Focus Group.
             *
             * Add Widget to Focus Group. The pointer to the Widget
             * has to be valid for the entire life time of the Focus
             * Group.
             *
             * @param _w pointer to the Widget.
             */
            void add(WidgetBase* _w);

            /**
             * Remove Widget from Focus Group.
             *
             * Remove Widget from Focus Group.
             *
             * @param _w pointer to Widget to be removed.
             */
            void remove(WidgetBase* _w);

            /**
             * Focus next Widget.
             *
             * Focus next Widget in the list and refreshes both, the
             * Widget having had the focus and the newly focused Widget.
             */
            void focus_next();

            /**
             * Focus previous Widget.
             *
             * Focus previous Widget in the list and refreshes both, the
             * Widget having had the focus and the newly focused Widget.
             */
            void focus_previous();

            /**
             * Re-focus the Widget having the focus.
             */
            void refocus() const;

            /**
             * Reset the focus.
             *
             * Resetting focus will result in first widget in the focus
             * list receiving focus.
             */
            void reset();
    };
}

#endif // FOCUSGROUP_H
