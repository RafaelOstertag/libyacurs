// -*- mode: c++ -*-
//
// $Id$

#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mycurses.h"
#include "realizeable.h"
#include "area.h"
#include "window.h"

// Forward declaration because window.h already included widgetbase.h
class Window;

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
 * If widgets use curses windows, it is expected that they create the window
 * using derwin() to create them.
 *
 */
class WidgetBase: public Realizeable {
    private:
	/**
	 * Curses window used to display the widget. Widgets like Pack may not
	 * directly use this, but pass the pointer on to assigned widgets.  The
	 * widget does not maintain the window, i.e. it does not free the
	 * memory occupied. However, the pointer has to be valid for the entire
	 * lifetime of the object.
	 */
	WINDOW* __curseswindow;
	/**
	 * The parent of the widget. If the widget has no parent, it has to be
	 * set to NULL.
	 *
	 * Basically, only container widgets such as Packs can be parents.
	 */
	WidgetBase* __parent;
	/**
	 * The parent Window of the widget.
	 *
	 * This is used for container widgets, so that they are able to query
	 * the area they have to their disposition.
	 */
	Window* __window;

	/**
	 * Holds the position, where the widget will display.
	 *
	 * Depending on the implemntation, those coordinates are relative to
	 * __curseswindow, since it is expected that widgets use derwin() if
	 * they display. Container widgets may use absolute coordinates.
	 */
	Coordinates __position;

	/**
	 * Holds the Size available to the widget.
	 *
	 * Holds the Size available to the widget starting from __position.
	 *
	 * This is mostly used for container widgets such as Pack or widgets
	 * dynamically adjust their size.
	 */
	Size __size_available;

    public:
	WidgetBase();
	~WidgetBase();
	WidgetBase(const WidgetBase& _w);
	WidgetBase& operator=(const WidgetBase& w);

	/**
	 * Set the position, where the widget will be displayed.
	 *
	 * @param _c position where the widget will be displayed. The position
	 * is relative to __curseswindow, since it is expected that widgets use
	 * derwin().
	 */
	void position(const Coordinates& _c);

	/**
	 * Get the the position of the widget.
	 *
	 * @return the position of the widget. The position is relative to the
	 * position of __curseswindow.
	 */
	const Coordinates& position() const;

	/**
	 * Set the screen Area available to the widget.
	 *
	 * This is used by container widgets.
	 *
	 * @param _a Area available to the widget
	 *
	 */
	void size_available(const Size& _s);

	/**
	 * Get the size available to the widget.
	 *
	 * Used by container widgets
	 *
	 * @return size available to the widget.
	 */
	const Size& size_available() const;

	/**
	 * Set parent of widget.
	 *
	 * A parent is usually a container widget such as Packs.
	 *
	 * This is usually not called by the user.
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

	/**
	 * Set the Window the widget belongs to
	 *
	 * @param _w pointer to the Window the widget belongs to. The
	 * pointer has to be valid for the lifetime of the object.
	 */
	void window(Window* _w);

	/**
	 * Get the Window the widget belongs to
	 *
	 * @return pointer to the Window the widget belongs to
	 */
	Window* window() const;

	/**
	 * Set the curses window of the widget.
	 *
	 * The curses window is used for displaying the widget.
	 *
	 * This is usually not called by the user.
	 *
	 * @param _p pointer to curses window. The pointer has to be valid for
	 * the entire lifetime of the widget.
	 */
	virtual void curseswindow(WINDOW* _p);

	/**
	 * Get the curses window.
	 *
	 * @return pointer to the curses window.
	 */
	WINDOW* curseswindow() const;

	/**
	 * Notification of size change of a child.
	 *
	 * This is called on a parent widget when the child detects a size
	 * change.
	 *
	 * If the called class is not a container
	 *
	 * @return the parent returns @c true if the size change can be
	 * fullfilled, else @false.
	 */
	virtual bool sizechange() = 0;

	/**
	 * The size the widget effectively occupies.
	 *
	 * @return size the widget effectively occupies.
	 */
	virtual const Size& size() const = 0;
};

#endif // WIDGETBASE_H
