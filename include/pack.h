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

class Pack: public Widget {
    private:
	std::list<Widget*> widget_list;

    protected:
	Rectangle __size;

	/**
	 * This function calculates the size required by all the
	 * widgets associated with the pack and updates __size
	 *
	 * Depending on the alignment (horizontally, vertically), the
	 * size is different. Thus it will be implemented in derrived
	 * classes.
	 */
	virtual void update_size() = 0;

    public:
	Pack();
	~Pack();
	Pack(const Pack& _p);
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
	void remove(const Widget* _w);
	/**
	 * @return the space required for displaying the pack and
	 * associated widget.
	 */
	const Rectangle& size();
};

#endif // PACK_H
