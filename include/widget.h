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
	 * curses subwin used by widgets
	 */
	WINDOW* __widget_subwin;

	// Not supported
	Widget& operator=(const Widget&);
    protected:
	void force_refresh_handler(Event& _e);

	void unrealize();

	WINDOW* widget_subwin() const;

    public:
	Widget();

	virtual ~Widget();


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
};
#endif // WIDGET_H
