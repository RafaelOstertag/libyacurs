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
 * using subwin() to create them.
 *
 * Information on how much space the widget has to its disposition, has to be
 * provided by the Window the widget is associated or by the parent Widget.
 *
 * Dynamically sized Widgets return Size::zero() upon a call to
 * size(). An implementation detail of those Widget is, that they
 * should reset their size() to Size::zero() upon unrealize().
 */
class WidgetBase: public Realizeable {
    private:
	/**
	 * Curses window used to display the widget. Widgets like Pack may not
	 * directly use this, but pass the pointer on to assigned widgets.  The
	 * widget does not maintain the window, i.e. it does not free the
	 * memory occupied. However, the pointer has to be valid for the entire
	 * lifetime of the object.
	 *
	 * Further, this pointer is used for subwin() in order to create
	 * subwindows.
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
	 * Holds the position on the screen, where the widget will display.
	 */
	Coordinates __position;

	/**
	 * Holds the Size available to the widget.
	 *
	 * Holds the Size available to the widget starting from
	 * __position.
	 *
	 * This is mostly used for container widgets such as Pack or
	 * Widgets dynamically adjust their size.
	 *
	 * This Size is also imperative for the Widget. If the size()
	 * of a Widget is component wise greater than
	 * __size_available, the behavior is undefined.
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
	 * Set the size available to the widget.
	 *
	 * This is used by container widgets and has to be set by the paren
	 * widget, or the Window if the widget has no parent Widget.
	 *
	 * @param _s size available to the widget
	 *
	 */
	virtual void size_available(const Size& _s);

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
	 * If the Widget returns a size of Size(0,0) it indicates that
	 * it dynamically adjusts to the size_available(). It then
	 * means, that the parent Widget has to provide a sensible
	 * value for size_available().
	 *
	 * size_hint() may provide a hint for the size occupied.
	 *
	 * @return size the widget effectively occupies, or Size(0,0)
	 * if it dynamically adjusts to the size_available().
	 */
	virtual Size size() const = 0;

	/**
	 * Provide a size hint.
	 *
	 * Dynamically size Widgets can provide size hints if possible.
	 *
	 * @return Size::zero() if no hint can be provided. Either
	 * Size::rows()>0 or Size::cols()>0 which is taken as
	 * hint. WidgetBase::size_hint() == WidgetBase::size() for
	 * non-dynamically sized Widgets.
	 */
	virtual Size size_hint() const = 0;

	/**
	 * Reset the size of the widget.
	 *
	 * This is used mainly for dynamically sized Widgets, so that will
	 * return Size(0,0) upon next call to size().
	 *
	 * Non-dynamically sized Widget are supposed to ignore this.
	 */
	virtual void resetsize() = 0;
};

#endif // WIDGETBASE_H
