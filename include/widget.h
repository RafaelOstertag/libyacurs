// -*- mode: c++ -*-

#ifndef WIDGET_H
#define WIDGET_H

#include <cstdlib>
#include <string>

#include "widgetbase.h"
#include "mycurses.h"

namespace YACURS {
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
	    YACURS::INTERNAL::CursWin* __widget_subwin;

	    // Not supported
	    Widget& operator=(const Widget&);
	protected:
	    void force_refresh_handler(Event& _e);

	    void unrealize();

	    YACURS::INTERNAL::CursWin* widget_subwin() const;

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
}
#endif // WIDGET_H
