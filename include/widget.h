// -*- mode: c++ -*-
//
// $Id$

#ifndef WIDGET_H
#define WIDGET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "realizeable.h"
#include "rectangle.h"

class Widget: public Realizeable {
    private:
	/**
	 * The parent of the widget. If the widget has no parent, it
	 * has to be set to NULL.
	 */
	Widget* __parent;
	Rectangle __area;


    public:
	Widget();
	~Widget();
	Widget(const Widget& _w);
	
	/**
	 * Set the screen area, the widget has to its disposition.
	 *
	 * @param _a area the widget has to its disposition. Its
	 * absolute with regards to stdscr.
	 */
	void area(const Rectangle& _a);
	/**
	 * Get the screen area, the widget has to its disposition.
	 *
	 * @return area the widget claims it has to its
	 * disposition. Absolute with regards to stdscr.
	 */
	const Rectangle& area() const;
	/**
	 * Return the size the widget uses effectively.
	 *
	 * @return the size the widget uses effectively.
	 */
	virtual const Rectangle& size() const = 0;
	virtual Widget* clone() const = 0;

};

#endif // WIDGET_H
