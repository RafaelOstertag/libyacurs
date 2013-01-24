// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDEXCEPT
#include <stdexcept>
#endif // HAVE_STDEXCEPT

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#include "curs.h"
#include "window.h"
#include "area.h"
#include "eventqueue.h"

//
// Private
//

//
// Protected
//
void
Window::unrealize() {
    if (__widget) __widget->unrealize();
    WindowBase::unrealize();
}

int 
Window::key_handler(Event& _e) {
    assert(_e == EVT_KEY);
    assert(realized());

    return -1;
}

//
// Public
//

Window::Window(const Margin& m): WindowBase(m), __widget(NULL) {
    EventQueue::connect_event(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window::Window(const Window& W): WindowBase(W),
				 __widget(W.__widget) {
    EventQueue::connect_event(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window::~Window() {
    EventQueue::disconnect_event(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

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
    WindowBase::refresh(immediate);
    if (__widget) __widget->refresh(immediate);
}

void
Window::resize(const Area& _a) {
    WindowBase::resize(_a);
    // We do not call resize, because the WidgetBase::resize() would
    // happen after the widget has been unrealized and realized again
    // by Window::resize().
    //
    // Window::realize takes care of setting up things
    //if (__widget) __widget->resize(widget_area());
}

void
Window::realize() {
    WindowBase::realize();
    if (__widget) {
	// This is imperative, so that the widget also is aware of the new
	// curses window in case we're called in the course of a resize.
	__widget->curses_window(curses_window());
	
	// This widget does not have another widget as parent.
	__widget->parent(NULL);
	
	__widget->position(widget_area());
	
	__widget->size_available(widget_area());

	__widget->realize();
    }
}
