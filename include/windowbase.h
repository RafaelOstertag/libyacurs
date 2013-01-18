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
 * - connect to EVT_REFRESH and EVT_RESIZE
 *
 * - allocates only one instance of curses Windows across
 *   assignments.
 *
 * - tracks the number of WindowBase instances created by copying and
 *   assigning and frees curses window only upon destruction of
 *   last instance.
 *
 *  - connect to the refresh (EVT_REFRESH) and resize (EVT_RESIZE) event.
 */
class WindowBase : public Realizeable {
    private:
	/**
	 * The dimension of the window. Used as cache to avoid calls to
	 * Curses::inquiryScreenSize().
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
	unsigned int* instances;
	/**
	 * Pointer to a pointer so that we can replace the window for
	 * all instances simultaneously.
	 */
	WINDOW** w;

	/**
	 * Whether or not the window has a border
	 */
	bool __frame;

    protected:
	WINDOW* getWindow() const;

	/// Keep this. Used by tests/windowrefs.cc.
	unsigned int getInstanceCount() const;

	const Area& area() const;

	void unrealize();

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
	 * Get the position from where other objects may be displayed.
	 *
	 * @return position from where other objects may be displayed.
	 */
	virtual Coordinates position_available() const;

	/**
	 * Get the size available for displaying other objects.
	 *
	 * @return size available for displaying other object.
	 */
	virtual Size size_available() const;

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

	// Those are from Realizable
	void refresh(bool immediate);
	void resize(const Area& _a);
	void realize();

};

#endif // WINDOWBASE_H
