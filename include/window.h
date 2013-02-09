// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOW_H
#define WINDOW_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "mycurses.h"
#include "windowbase.h"
#include "widgetbase.h"

// Forward declaration because widgetbase.h already included window.h
class WidgetBase;

/**
 * Window for displaying a widget.
 *
 * Several Widgets can be displayed by the use of Packs.
 *
 * Upon realization, it will create a new Focus Group. When
 * unrealized, it will destroy the Focus Group. This ensures that
 * Widgets will register to the proper Focus Group upon their
 * realization.
 */
class Window: public WindowBase {
    private:
	WidgetBase* __widget;

	/**
	 * ID of the Focus Group.
	 *
	 * The ID of the Focus Group belonging to this
	 * Window. Supposed to be passed along Widget(s).
	 */
	fgid_t __fgid;

    protected:
	void unrealize();

    public:
	Window(const Margin& m=Margin());
	Window(const Window& W);
	virtual ~Window();
	Window& operator=(const Window& W);
	
	void widget(WidgetBase* _w);
	WidgetBase* widget() const;

	// Those are from Realizable
	void refresh(bool immediate);
	// Does nothing, everything handled in parent.
	//void resize(const Area& _a);
	void realize();

	operator std::string() const;
};

#endif // WINDOW_H
