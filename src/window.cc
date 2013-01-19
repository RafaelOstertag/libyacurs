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
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window::Window(const Window& W): WindowBase(W),
				 __widget(W.__widget) {
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window::~Window() {
    EventQueue::disconnectEvent(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
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

    _w->curseswindow(getWindow());

    // This widget does not have another widget as parent.
    _w->parent(NULL);

    _w->window(this);

    _w->position(widget_area());
    _w->size_available(widget_area());
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
    if (__widget) __widget->resize(widget_area());
}

void
Window::realize() {
    WindowBase::realize();
    if (__widget) {
	// This is imperative, so that the widget also is aware of the new
	// curses window in case we're calles in the course of a resize.
	__widget->curseswindow(getWindow());
	__widget->realize();
    }
}
