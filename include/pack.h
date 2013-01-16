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

#include "rectangle.h"
#include "widget.h"

/**
 * A pack is a list of widgets, that are either horizontally, or vertically
 * stacked and displayed.
 *
 * The pack will calculate the size needed for displaying the widgets
 * horizontally, vertically.
 */
class Pack: public Widget {
    private:
	std::list<Widget*> widget_list;

    protected:
	/**
	 * The size requirements of all the widgets in the pack.
	 */
	Rectangle __size;

	/**
	 * Add the size of the given widget to the __size.
	 *
	 * Depending on the alignment (horizontally, vertically), the
	 * size is different. Thus it will be implemented in derrived
	 * classes.
	 *
	 * @param _w the widget the size has to taken into account.
	 */
	virtual void add_size(const Widget* _w) = 0;

	/**
	 * Recalculates the size requirements of all widgets in the pack.
	 *
	 * Needed to recalculate the size when removing widgets from the pack.
	 *
	 * @internal Simply subtracting the size of the widget to be removed
	 * from __size, might not always do the trick, e.g. removing a widget
	 * with size requirement of 4x4 from a pack consisting of only of 2x2
	 * will leave 2 rows/cols (depending on vertical or horizontal packing)
	 * too many in __size. So we have to take all remaining widgets into
	 * account as well.
	 */
	virtual void recalc_size() = 0;

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
	void add_front(Widget* _w);
	/**
	 * Add a widget to the Pack. The widget will be added after
	 * all other widget.
	 *
	 * @param _w the widget to add. The pack does not create a
	 * copy, so the pointer has to remain valid for the lifetime
	 * of the pack.
	 */
	void add_back(Widget* _w);
	/**
	 * Remove a previously added widget.
	 *
	 * @param _w widget to be removed. Pack compares the address
	 * of the widget.
	 */
	void remove(Widget* _w);
	/**
	 * @return the space required for displaying the pack and
	 * associated widget.
	 */
	const Rectangle& size() const;
};

#endif // PACK_H