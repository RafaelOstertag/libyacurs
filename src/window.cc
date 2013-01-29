// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>
#include <cassert>


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
    if (!realized()) return;

    if (__widget) __widget->unrealize();

    FocusManager::destroy_focus_group();
    WindowBase::unrealize();
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
    if (!realized()) return;

    YAPET::UI::Colors::set_color(curses_window(), YAPET::UI::DEFAULT);
    WindowBase::refresh(immediate);
    if (__widget) __widget->refresh(immediate);
}

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

void
Window::realize() {
    WindowBase::realize();

    // It is imperative that a new Focus Group is created before the
    // Widget is realized()!
    FocusManager::new_focus_group();

    if (__widget) {
	assert(!__widget->realized());

	// This is imperative, so that the widget also is aware of the
	// new curses window in case we're called in the course of a
	// resize.
	__widget->curses_window(curses_window());

	// This widget does not have another widget as parent.
	__widget->parent(NULL);

	__widget->position(widget_area());

	__widget->size_available(widget_area());

	__widget->realize();
    }
}
