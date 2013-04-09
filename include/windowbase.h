// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOWBASE_H
#define WINDOWBASE_H

#include "curswin.h"
#include "realizeable.h"
#include "margin.h"
#include "event.h"
#include "mycurses.h"

namespace YACURS {

    enum {
	MIN_WINDOW_ROWS=1,
	MIN_WINDOW_COLS=4
    };

    /**
     * Base class for Windows.
     *
     * WindowBase has the following properties:
     *
     * - curses window uses entire screen, unless a margin is given
     *
     * - positioning is achieved by using margins
     *
     * - can have a visible frame
     *
     * - connect to EVT_REFRESH, EVT_FORCEREFRESH and EVT_RESIZE
     *
     * - allocates only one instance of curses Windows across
     *   assignments.
     *
     * - tracks the number of WindowBase instances created by copying and
     *   assigning and frees curses window only upon destruction of
     *   last instance.
     *
     * Users of this class are expected to call show()/close() in order to
     * show and hide the window. Calling realize()/unrealize() is
     * discouraged.
     *
     * Events will be connected upon call to show() and disconnected upon
     * call to close().
     *
     * Derived classes do not need to connect to EVT_REFRESH, or
     * EVT_RESIZE. The Event handler of WindowBase call refresh() or
     * resize() which can be implemented by derived classes.
     */
    class WindowBase : public Realizeable {
	private:
	    /**
	     * The dimension of the screen. Used as cache to avoid calls
	     * to Curses::inquiry_screensize().
	     */
	    Area __area;

	    /**
	     * margin of the screen object.
	     */
	    Margin __margin;

	    /**
	     * Curses window
	     */
	    YACURS::INTERNAL::CursWin* __curses_window;

	    /**
	     * Whether or not the window has a border
	     */
	    bool __frame;

	    /**
	     * Flag whether or not Window is shown.
	     */
	    bool __shown;

	    // No supported
	    WindowBase(const WindowBase&);
	    WindowBase& operator=(const WindowBase&);
	protected:
	    YACURS::INTERNAL::CursWin* curses_window() const;

	    const Area& area() const;
	    /**
	     * Gets the area that can be used by widgets
	     *
	     * @return the area in with absolute coordinates that can be
	     * used by widgets.
	     */
	    Area widget_area() const;

	    void unrealize();

	public:
	    /**
	     * @param _m margin to be used.
	     */
	    WindowBase(const Margin& _m = Margin());
	    virtual ~WindowBase();


	    /**
	     * @todo when setting margin and window is realized, resize
	     * it.
	     */
	    void margin(const Margin& _m);
	    const Margin& margin() const;

	    bool frame() const;

	    /**
	     * @todo make setting/removing frame take effect immediately,
	     * if window is realized.
	     */
	    void frame(bool b);

	    /**
	     * Show the window.
	     *
	     * This is supposed to be called by the user.
	     */
	    virtual void show();

	    /**
	     * Hide the window.
	     *
	     * This is supposed to be called by the user.
	     */
	    virtual void close();

	    bool shown() const;

	    virtual void force_refresh_handler(Event& _e);
	    virtual void refresh_handler(Event& _e);
	    virtual void resize_handler(Event& _e);

	    // Those are from Realizable
	    void refresh(bool immediate);
	    void resize(const Area& _a);
	    void realize();
    };
}

#endif // WINDOWBASE_H
