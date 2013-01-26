// -*- mode: c++ -*-
//
// $Id$

#ifndef FOCUSGROUP_H
#define FOCUSGROUP_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>

#include "widgetbase.h"

/**
 * Focus Group
 *
 * A Focus Group is comprised of Widgets with one Widget having the
 * focus, if the Group is active (or the Current Focus Group with
 * regards to the Focus Manager).
 *
 * Everytime a focus change happens, an EVT_FOCUS_CHANGE will be
 * emitted in order to make Widgets aware of focus changes.
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
	FocusGroup();
	FocusGroup(const FocusGroup& _f);
	~FocusGroup();
	FocusGroup& operator=(const FocusGroup& _f);

	/**
	 * Activate Focus Group
	 *
	 * When the Focus Group is activated, the Widget having the
	 * focus last in the Focus Group before deactivation, (or the
	 * first on initial call) will receive the Focus.
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
	 * Focus next Widget in the list.
	 */
	void focus_next();

	/**
	 * Focus previous Widget.
	 *
	 * Focus previous Widget in the list.
	 */
	void focus_previous();

};

#endif // FOCUSGROUP_H
