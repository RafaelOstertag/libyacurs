// -*- mode: c++ -*-
//
// $Id$

#ifndef FOCUSMANAGER_H
#define FOCUSMANAGER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stack>
#include <list>

#include "focusgroup.h"
#include "widgetbase.h"

/**
 * @defgroup Focus Focus Management
 *
 * Focus Groups and Focus Manager.
 *
 * Focus Management is comprised of Focus Groups and the Focus
 * Manager. Focus Groups are organized in a stack, where the top most
 * Focus Group is the Current Focus Group.
 *
 * The Focus Manager maintains the stack of Focus Groups and is the
 * only interface to Focus Groups.
 */

/**
 * @ingroup Focus
 *
 * Focus Manager for Widgets.
 *
 * In Libyacurs, only widgets can be focused. Widgets register
 * themselves upon realization with the current Focus Group, which is
 * maintained by the FocusManager.
 *
 * A Window does create a Focus Group upon realization, which then
 * becomes the Current Focus Group. Since a Window is always realized
 * before Widgets, Widgets register themselves automatically with the
 * proper Focus Group.
 *
 * Focus Groups are organized in a stack. Windows basically push and
 * pop on realization and unrealization Focus Groups of that stack.
 *
 * The FocusManager is intended to be the only interface to
 * FocusGroups for other objects.
 *
 * @sa FocusGroup
 */
class FocusManager {
    private:
	/**
	 * Stack holding Focus Groups. The top of the stack is the
	 * current Focus Group.
	 */
	static std::stack<FocusGroup*> __focus_groups;

	/**
	 * Handles EVT_FOCUS_NEXT and EVT_FOCUS_PREVIOUS.
	 */
	static int focus_change_handler(Event& _e);

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
	 * Create a new Focus Group, which will become the Current
	 * Focus Group.
	 */
	static void new_focus_group();

	/**
	 * Destroy Current Focus Group.
	 *
	 * Destroy Current Focus Group. The previous Focus Group
	 * becomes the Current Focus Group.
	 */
	static void destroy_focus_group();

	/**
	 * Register Widgets with Current Focus Group
	 *
	 * The pointer to the Widget has to be valid for the entire
	 * lifetime of the Current Focus Group.
	 *
	 * @param _w pointer to the Widget.
	 */
	static void current_focus_group_add(WidgetBase* _w);

	/**
	 * Unregister Widgets from Current Focus Group
	 *
	 * @param _w pointer to the Widget.
	 */
	static void current_focus_group_remove(WidgetBase* _w);

	/**
	 * Re-focus
	 *
	 * Calls refocus() on Current Focus Group.
	 */
	static void refocus();
};

#endif // FOCUSMANAGER_H
