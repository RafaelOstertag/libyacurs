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
 * - positioning is achieved by using margins
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
	/// The dimension of screen object
	Area rect;
	/**
	 * margin of the screen object.
	 */
	Margin margin;
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

    protected:
	WINDOW* getWindow() const;

	/// Keep this. Used by tests/windowrefs.cc.
	unsigned int getInstanceCount() const;

	/**
	 * @todo when setting margin and window is realized, resize
	 * it.
	 */
	void setMargin(const Margin& _m);
	const Margin& getMargin() const;

	void unrealize();

    public:
	/**
	 * @param _a size and position of the window. If it is equal
	 * to Area(), WindowBase will retrieve the screen size by
	 * calling Curses::inquiryScreenSize().
	 */
	WindowBase(const Area& _a = Area(),
		     const Margin& _m = Margin());
	WindowBase(const WindowBase& so);
	virtual ~WindowBase();
	WindowBase& operator=(const WindowBase& so);

	void refresh(bool immediate);
	void resize(const Size& _s);
	void realize();

};

#endif // WINDOWBASE_H
