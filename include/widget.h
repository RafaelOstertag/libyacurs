// -*- mode: c++ -*-

#ifndef WIDGET_H
#define WIDGET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>
#include <string>

#include "mycurses.h"
#include "widgetbase.h"


/**
 * Implements a Widget.
 *
 * @subsection Basics
 *
 * Upon realize(), only the Curses Window will be created. refresh()
 * only calls wrefresh() or wnoutrefresh(). unrealize() will destroy
 * the Curses Window.
 *
 * @subsection Content
 *
 * Content in Curses Window should only be drawn upon refresh().
 */
class Widget: public WidgetBase {
    private:
	/**
	 * Keeps track of how many instance objects have been created
	 * sharing the same WINDOW structure
	 */
	unsigned int* __instance_count;
	/**
	 * Pointer to a pointer so that we can replace the window for
	 * all instances simultaneously.
	 */
	WINDOW** __widget_subwin;

    protected:
	void force_refresh_handler(Event& _e);

	void unrealize();

	WINDOW* widget_subwin() const;

    public:
	Widget();

	/**
	 * Copy constructor.
	 *
	 * @param _w reference to Widget object.
	 */
	Widget(const Widget& _w);

	virtual ~Widget();

	/**
	 * Assignment operator.
	 *
	 * @param _w reference to Widget object.
	 */
	Widget& operator=(const Widget& _w);

	// Must be overriden in derived classes
	//bool size_change();

	// Inherited from Realizeable
	void refresh(bool immediate);
	void resize(const Area& _a);

	/**
	 * Realize the widget.
	 *
	 * realizes the widget by setting up a curses subwindow, which
	 * can be used by derived classes.
	 */
	void realize();

	/**
	 * Convert to std::string.
	 *
	 * Convert the object to a std::string. Used for debugging.
	 */
	operator std::string() const;
};


#endif // WIDGET_H
