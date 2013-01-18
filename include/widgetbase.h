// -*- mode: c++ -*-
//
// $Id$

#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "realizeable.h"
#include "area.h"

/**
 * A widget is an object with or without interaction on the screen.
 *
 * WidgetBase provides the interface common to all Widgets.
 *
 * The interface is comprised of the following:
 *
 *  - setting position where the widget will be displayed
 *  - query the size the widget requires
 *  - setting/unsetting focus
 *  - setting a parent widget
 *
 */
class WidgetBase: public Realizeable {
    private:
	/**
	 * The parent of the widget. If the widget has no parent, it has to be
	 * set to NULL.
	 */
	WidgetBase* __parent;
	/**
	 * Holds the position, where the widget will be displayed.
	 */
	Coordinates __position;

    protected:
	/**
	 * Holds  the size  in columns  and rows  that the  widgetbase
	 * requires.
	 *
	 * Derrived widgets have to maintain this attribute.
	 */
	Size __size;

    public:
	WidgetBase();
	~WidgetBase();
	WidgetBase(const WidgetBase& _w);
	const WidgetBase& operator=(const WidgetBase& w);
	
	/**
	 * Set the screen position, where the widget will be displayed.
	 *
	 * @param _c position where the widget will be displayed
	 */
	void position(const Coordinates& _c);
	/**
	 * Get the the position of the widget on the screen
	 *
	 * @return the position of the widget on the screen.
	 */
	const Size& position() const;
	/**
	 * Set parent of widget.
	 *
	 * @param _p the parent. The pointer has to be valid during
	 * the lifetime of the widget.
	 */
	void parent(WidgetBase* _p);
	/**
	 * Get pointer to the parent of the widget.
	 *
	 * If the widget has no parent, NULL is returned.
	 *
	 * @return the address of the parent widget or NULL if the is
	 * no parent.
	 */
	WidgetBase* parent() const;
	virtual WidgetBase* clone() const = 0;

};

#endif // WIDGETBASE_H
