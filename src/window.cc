// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>
#include <cassert>

#include "debug.h"

#include "curs.h"
#include "cursex.h"
#include "window.h"
#include "area.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "colors.h"

//
// Private
//

//
// Protected
//
void
Window::unrealize() {
    UNREALIZE_ENTER();

    DEBUGOUT("-- IN: Window::unrealize()");
    DEBUGOUT(*this);

    FocusManager::destroy_focus_group();

    if (__widget) __widget->unrealize();


    WindowBase::unrealize();

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: Window::unrealize()");

    UNREALIZE_LEAVE();
}

//
// Public
//

Window::Window(const Margin& m): WindowBase(m),
				 __widget(NULL) {}

Window::Window(const Window& W): WindowBase(W),
				 __widget(W.__widget) {}


Window::~Window() {}

Window&
Window::operator=(const Window& W) {
    WindowBase::operator=(W);

    __widget = W.__widget;

    return *this;
}

void
Window::widget(WidgetBase* _w) {
    __widget = _w;
}

WidgetBase*
Window::widget() const {
    return __widget;
}

void
Window::refresh(bool immediate) {
    if (realization()!=REALIZED) return;
    DEBUGOUT("-- IN: Window::refresh()");
    DEBUGOUT(*this);

    YAPET::UI::Colors::set_color(curses_window(), YAPET::UI::DEFAULT);

    WindowBase::refresh(immediate);

    if (__widget) __widget->refresh(immediate);

    DEBUGOUT("-- OUT: Window::refresh()");
    DEBUGOUT(*this);
}

#if 0
// This ain't needed since it does nothing...
void
Window::resize(const Area& _a) {
    WindowBase::resize(_a);

    // We do not call resize, because the WidgetBase::resize() would
    // happen after the widget has been unrealized and realized again
    // by Window::resize(). Therefore, Window::realize takes care of
    // setting up things
    //
    //if (__widget) __widget->resize(widget_area());
}
#endif

/**
 * @todo widget does not show cursor on focus after realization.
 */
void
Window::realize() {
    REALIZE_ENTER();

    DEBUGOUT("-- IN: Window::realize()");
    DEBUGOUT(*this);

    WindowBase::realize();

    // It is imperative that a new Focus Group is created before the
    // Widget is realized()!
    FocusManager::new_focus_group();

    if (__widget) {
	assert(__widget->realization()==UNREALIZED);

	// This is imperative, so that the widget also is aware of the
	// new curses window in case we're called in the course of a
	// resize.
	__widget->curses_window(curses_window());

	// This widget does not have another widget as parent.
	__widget->parent(NULL);

	__widget->position(widget_area());

	__widget->size_available(widget_area());

	__widget->realize();

	// Make sure a widget has the focus
	FocusManager::refocus();
    }

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: Window::realize()");

    REALIZE_LEAVE();
}

Window::operator std::string() const {
    return "Window{\n\t(__widget_area:" + 
	static_cast<std::string>(widget_area()) + ")\n}";
}
