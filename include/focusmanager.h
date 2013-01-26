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
 * Focus Manager for Widgets.
 *
 * In Libyacurs, only widgets can be focused. Widgets register
 * themselves upon realization with the current focus group, which is
 * maintained by the FocusManager.
 *
 * A Window does create a focus group upon realization, which then
 * becomes the current focus group. Since a Window is always realized
 * before Widgets, Widgets register themselves automatically with the
 * proper focus group.
 */
class FocusManager {
    private:
	/**
	 * Stack holding Focus Groups. The top of the stack is the
	 * current Focus Group.
	 */
	static std::stack<FocusGroup*> __focus_groups;

    public:
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
	 * Focus next Widget in Current Focus Group.
	 */
	static void focus_next();

	/**
	 * Focus previous Widget in Current Focus Group.
	 */
	static void focus_previous();
};

#endif // FOCUSMANAGER_H
