// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include "mycurses.h"
#include "windowbase.h"
#include "widgetbase.h"

namespace YACURS {
    // Forward declaration because widgetbase.h already included window.h
    class WidgetBase;

    /**
     * Window for displaying a widget.
     *
     * @subsection Widget
     *
     * Window is responsible for realizing/unrealizing/refresh the
     * Widget.
     *
     * A screen resize will result in Window calling resize() of the Widget.
     *
     * Several Widgets can be displayed by the use of Packs.
     *
     * Window will call set
     *
     *  - Curses Window (curses_window())
     *  - Focus Group (focusgroup_id())
     *  - Position (position())
     *  - Available Size (size_available())
     *
     * @subsection Focus
     *
     * Upon realization, Window will create a new Focus Group. When
     * unrealized, it will destroy the Focus Group.
     *
     * Window is responsible for passing along Focus Group ID to
     * Widget.
     *
     * Window will activate its Focus Group upon refresh(). This ensures
     * that the last Window refresh()'ed will have the focus.
     *
     * @subsection Events
     *
     * Window does not connect any Events explicitely. It implements
     * unrealize(), refresh(), and realize(), which will be called by
     * Event Handlers of WindowBase.
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

	    // Not supported
	    Window(const Window&);
	    Window& operator=(const Window&);
	protected:
	    void unrealize();

	public:
	    Window(const Margin& m=Margin());
	    virtual ~Window();
	
	    void widget(WidgetBase* _w);
	    WidgetBase* widget() const;

	    // Those are from Realizable
	    void refresh(bool immediate);
	    // Does nothing, everything handled in parent.
	    //void resize(const Area& _a);
	    void realize();
    };
}

#endif // WINDOW_H
