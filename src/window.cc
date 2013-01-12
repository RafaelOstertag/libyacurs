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
#include "rectangle.h"
#include "eventqueue.h"

//
// Private
//

//
// Protected
//
int 
Window::refresh_handler(EventBase& _e) {
    assert(_e == EVT_REFRESH);
    assert(isRealized());
    refresh(false);
    return 0;
}

int 
Window::resize_handler(EventBase& _e) {
    assert(_e == EVT_WINCH);
    assert(isRealized());
    
    EventWinCh& winch = dynamic_cast<EventWinCh&>(_e);
    resize(winch.data());
    
    return 0;
}

int 
Window::key_handler(EventBase& _e) {
    assert(_e == EVT_KEY);
    assert(isRealized());

    return -1;
}

//
// Public
//

Window::Window(const Margin<>& m):
    ScreenObject(Curses::inquiryScreenSize(), m),
    hasframe(false) {

    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_REFRESH,this, &Window::refresh_handler));
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_WINCH,this, &Window::resize_handler));
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window::Window():
    ScreenObject(Curses::inquiryScreenSize()),
    hasframe(false) {
}

Window::Window(const Window& W):
    ScreenObject(W), hasframe(W.hasframe) {
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_REFRESH,this, &Window::refresh_handler));
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_WINCH,this, &Window::resize_handler));
    EventQueue::connectEvent(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window::~Window() {
    EventQueue::disconnectEvent(EventConnectorMethod1<Window>(EVT_REFRESH,this, &Window::refresh_handler));
    EventQueue::disconnectEvent(EventConnectorMethod1<Window>(EVT_WINCH,this, &Window::resize_handler));
    EventQueue::disconnectEvent(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_handler));
}

Window&
Window::operator=(const Window& W) {
    ScreenObject::operator=(W);

    hasframe = W.hasframe;

    return *this;
}

void
Window::realize() {
    ScreenObject::realize();

    if (hasframe) {
	if (box(getWindow(), 0, 0) == ERR)
	    throw BoxFailed();
    }
}

bool
Window::getFrame() const {
    return hasframe;
}

void
Window::setFrame(bool b) {
    hasframe = b;
}
