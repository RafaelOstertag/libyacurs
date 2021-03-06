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

#ifndef FOCUSMANAGER_H
#define FOCUSMANAGER_H 1

#include <vector>

#include "focusgroup.h"
#include "yacurstypes.h"

namespace YACURS {
class WidgetBase;

/**
 * @defgroup Focus Focus Management
 *
 * Focus Groups and Focus Manager.
 *
 * Focus Management is comprised of Focus Groups and the
 * Focus Manager. Focus Groups are organized in a list by
 * the Focus Manager, where only one Focus Group can be
 * active, aka the Current Focus Group.
 */

/**
 * @ingroup Focus
 *
 * Focus Manager for Widgets.
 *
 * In Libyacurs, only widgets can be focused. Widgets register
 * themselves upon realization with a Focus Group identified by a
 * Focus Group ID.
 *
 * A Window does create a Focus Group upon realization an receives a
 * Focus Group ID. The Window is then responsible for passing on the
 * Focus Group ID to Widgets in order to let the Widgets register
 * themselves with the proper Focus Group.
 *
 * The Window has to activate the Focus Group upon refresh(), which
 * ensures that the last refreshed Window will have set its Focus
 * Group active, which is supposed to be top most Window, given the
 * EventQueue maintains EventConnectors in the order they were
 * connected.
 *
 * Focus Groups are organized in a vector. Windows are supposed to
 * activate their Focus Group on refresh().
 *
 * The FocusManager is intended to be the only interface to
 * FocusGroups for other objects.
 *
 * In a nutshell: a Window
 *
 *  - creates a new Focus Group upon realize().
 *  - destroys the Focus Group upon unrealize().
 *  - activates the Focus Group upon refresh().
 *
 * @note Focus Group IDs are zero-based.
 *
 * @sa FocusGroup
 *
 * @internal
 *
 * Early implementations used to maintain a stack and did not
 * employ Focus Group IDs. This works, as long as no screen resize
 * occurs, which can cause Widgets in obscured Windows to
 * unrealize() due to size constraints. Those Widgets would then
 * remove themselves from the Current Focus Group which is not the
 * proper one, since it belongs to the top most Window. So, the
 * implementation now requires to activate the Focus Group on a
 * Window refresh(), since the Window is refreshed before Widgets
 * are, so we maintain activation of proper Focus Group.
 *
 * The Focus Group ID is simply the index of the Focus Group in
 * the vector. We don't remove items from the vector, but set them
 * to @c 0 upon destruction, so that we can identify empty slots.
 */
class FocusManager {
   public:
    typedef std::vector<FocusGroup*>::size_type fgid_t;

   private:
    /**
     * Holds the Focus Group ID of the Active Focus Group. It is
     * legal to hold a Focus Group ID of a Group been
     * removed. focus_group_activate() has to cope with that.
     */
    static fgid_t _active_focusgroup;

    /**
     * List holding _FocusGroupItem comprised of a Focus Group
     * and a Focus Group ID.
     *
     * Only one Focus Group may be active at a time, which is
     * always the las created Focus Group, i.e. the Focus Group
     * associated with the _FocusGroupItem added last.
     */
    static std::vector<FocusGroup*> _focus_groups;

    /**
     * Handles EVT_FOCUS_NEXT and EVT_FOCUS_PREVIOUS.
     */
    static void focus_change_handler(Event& _e);

   public:
    /**
     * initializes the Focus Manager.
     */
    static void init();

    /**
     * uninitializes the Focus Manager.
     */
    static void uninit();

    /**
     * Create a new Focus Group.
     *
     * @return the Focus Group ID to be used when adding/removing
     * Widgets to the Focus Group.
     */
    static fgid_t new_focus_group();

    /**
     * Destroy Current Focus Group.
     *
     * Destroy Current Focus Group.
     */
    static void destroy_focus_group(fgid_t id);

    /**
     * Register Widgets with a Focus Group
     *
     * The pointer to the Widget has to be valid for the entire
     * lifetime of the Current Focus Group.
     *
     * @param id Focus Group ID.
     *
     * @param w pointer to the Widget.
     */
    static void focus_group_add(fgid_t id, WidgetBase* w);

    /**
     * Unregister Widgets from Focus Group
     *
     * @param id Focus Group ID.
     *
     * @param w pointer to the Widget.
     */
    static void focus_group_remove(fgid_t id, WidgetBase* w);

    /**
     * Activate a given Focus Group.
     *
     * Activate the Focus Group identified by the ID and
     * deactivate all other Focus Groups.
     *
     * @param id Focus Group to activate.
     */
    static void focus_group_activate(fgid_t id);

    /**
     * Re-focus
     *
     * Calls refocus() on Active Focus Group.
     */
    static void refocus();

    /**
     * Reset the focus on the active Focus Group.
     */
    static void reset();

    /**
     * @return Focus Group ID of Active Focus Group.
     */
    static fgid_t active_focus_group();

    /**
     * No focus group id
     */
    static const fgid_t nfgid;
};
}  // namespace YACURS

#endif  // FOCUSMANAGER_H
