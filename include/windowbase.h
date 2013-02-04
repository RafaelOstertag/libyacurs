// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOWBASE_H
#define WINDOWBASE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mycurses.h"
#include "realizeable.h"
#include "margin.h"
#include "event.h"

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
	 * Keeps track of how many instance objects have been created
	 * sharing the same WINDOW structure
	 */
	unsigned int* __instance_count;

	/**
	 * Pointer to a pointer so that we can replace the window for
	 * all instances simultaneously.
	 */
	WINDOW** __curses_window;

	/**
	 * Whether or not the window has a border
	 */
	bool __frame;

    protected:
	WINDOW* curses_window() const;

	/// Keep this. Used by tests/windowrefs.cc.
	unsigned int instance_count() const;

	const Area& area() const;
	/**
	 * Gets the area that can be used by widgets
	 *
	 * @return the area in with absolute coordinates that can be
	 * used by widgets.
	 */
	Area widget_area() const;

	void unrealize();

	virtual int force_refresh_handler(Event& _e);
	virtual int refresh_handler(Event& _e);
	virtual int resize_handler(Event& _e);

    public:
	/**
	 * @param _m margin to be used.
	 */
	WindowBase(const Margin& _m = Margin());
	WindowBase(const WindowBase& so);
	virtual ~WindowBase();
	WindowBase& operator=(const WindowBase& so);

	/**
	 * @todo when setting margin and window is realized, resize
	 * it.
	 */
	void margin(const Margin& _m);
	const Margin& margin() const;

	bool frame() const;
	/**
	 * @todo make setting/removing frame take effect immediately, if window
	 * is realized.
	 */
	void frame(bool b);

	/**
	 * Show the window.
	 *
	 * This is supposed to be called by the user.
	 */
	virtual void show() = 0;

	/**
	 * Hide the window.
	 *
	 * This is supposed to be called by the user.
	 */
	virtual void hide() = 0;

	// Those are from Realizable
	void refresh(bool immediate);
	void resize(const Area& _a);
	void realize();

	operator std::string() const;
};

#endif // WINDOWBASE_H
