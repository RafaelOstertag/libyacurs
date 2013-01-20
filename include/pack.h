// -*- mode: c++ -*-
//
// $Id$

#ifndef PACK_H
#define PACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIST
#include <list>
#endif // HAVE_LIST

#include "area.h"
#include "widgetbase.h"

/**
 * A container widget for stacking widgets horizontally or vertically.
 *
 * A pack is a list of widgets, that are either horizontally, or vertically
 * stacked and displayed.
 *
 * The pack will calculate the size needed for displaying the widgets
 * horizontally, vertically. This should happen on realize() which is required
 * to be implemented in derived classes.
 *
 * @warning If a Pack is destroyed before its associated Widgets, the Widgets will have
 * a __parent pointer that is invalid.
 *
 *
 * @see VPack
 * @see HPack
 */
class Pack: public WidgetBase {
    private:
	/**
	 * The Size of the Pack.
	 *
	 * This will be set by calling size_available().
	 */
	Size __size;

	/**
	 * Call curseswindow() on all associated Widgets.
	 */
	void set_all_curseswindow();

    protected:
	/**
	 * List of all Widgets associated with the Pack.
	 */
	std::list<WidgetBase*> widget_list;

	/**
	 * Recalculates the size requirements of all widgets in the
	 * pack.
	 *
	 * Needed to recalculate the size when removing widgets from
	 * the pack.
	 *
	 * @internal Simply subtracting the size of the widget to be removed
	 * from __size, might not always do the trick, e.g. removing a widget
	 * with size requirement of 4x4 from a pack consisting of only of 2x2
	 * will leave 2 rows/cols (depending on vertical or horizontal packing)
	 * too many in __size. So we have to take all remaining widgets into
	 * account as well.
	 *
	 * @internal Pack::remove() uses this function.
	 */
	virtual void recalc_size() = 0;


	void unrealize();

    public:
	Pack();
	~Pack();
	Pack(const Pack& _p);
	const Pack& operator=(const Pack& _p);

	/**
	 * Add a widget to the Pack. The widget will be added in front
	 * of any other widget.
	 *
	 * @param _w the widget to add. The pack does not create a
	 * copy, so the pointer has to remain valid for the lifetime
	 * of the pack.
	 */
	void add_front(WidgetBase* _w);
	/**
	 * Add a widget to the Pack. The widget will be added after
	 * all other widget.
	 *
	 * @param _w the widget to add. The pack does not create a
	 * copy, so the pointer has to remain valid for the lifetime
	 * of the pack.
	 */
	void add_back(WidgetBase* _w);
	/**
	 * Remove a previously added widget.
	 *
	 * @param _w widget to be removed. Pack compares the address
	 * of the widget.
	 */
	void remove(WidgetBase* _w);

	/**
	 * Set the curses window of the pack and all associated widgets.
	 *
	 * Pack has to maintain its curses window pointer as well as the
	 * pointer of the associated widgets.
	 *
	 * @param _p pointer to curses window
	 */
	void curseswindow(WINDOW* _p);

	/**
	 * Set the available size.
	 *
	 * Since Pack is dynamically sizeable, we have to react on
	 * size_available(), unlike statically sized Widgets.
	 *
	 * @param _s the size available to the Pack.
	 */
	void size_available(const Size& _s);

	const Size& size() const;

	void resetsize();

	bool sizechange();

	/**
	 * Refresh all Widgets in the Pack.
	 *
	 * @param immediate @see Realizeable
	 */
	void refresh(bool immediate);
	/**
	 * Resize the pack.
	 *
	 * This function relies on the implementation of realize() of derived
	 * classes.
	 *
	 * @param _s the new size available to the pack.
	 */
	void resize(const Area& _a);

	//
	// Has to be implemented in derrived classes
	//void realize();
};

#endif // PACK_H
