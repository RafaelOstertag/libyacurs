// -*- mode: c++ -*-
//
// $Id$

#ifndef WIDGET_H
#define WIDGET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "realizeable.h"
#include "area.h"

class Widget: virtual public Realizeable {
    private:
	/**
	 * The parent of the widget. If the widget has no parent, it
	 * has to be set to NULL.
	 */
	Widget* __parent;
	Area __area;

    protected:
	/**
	 * Holds the size in columns and rows (w/ x=y=0) that the
	 * widget requires.
	 *
	 * Derrived widgets have to maintain this attribute.
	 */
	Area __size;

    public:
	Widget();
	~Widget();
	Widget(const Widget& _w);
	const Widget& operator=(const Widget& w);
	
	/**
	 * Set the screen area, the widget has to its disposition.
	 *
	 * @param _a area the widget has to its disposition. Its
	 * absolute with regards to stdscr.
	 */
	void area(const Area& _a);
	/**
	 * Get the screen area, the widget has to its disposition.
	 *
	 * @return area the widget claims it has to its
	 * disposition. Absolute with regards to stdscr.
	 */
	const Area& area() const;
	/**
	 * Set parent of widget.
	 *
	 * @param _p the parent. The pointer has to be valid during
	 * the lifetime of the widget.
	 */
	void parent(Widget* _p);
	/**
	 * Get the address parent of the widget.
	 *
	 * If the widget has no parent, NULL is returned.
	 *
	 * @return the address of the parent widget or NULL if the is
	 * no parent.
	 */
	Widget* parent() const;
	/**
	 * Return the size the widget uses effectively.
	 *
	 * @return the size the widget uses effectively.
	 */
	virtual const Area& size() const;
	virtual Widget* clone() const = 0;

};

#endif // WIDGET_H
