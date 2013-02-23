// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>
#include <cassert>
#include <cstdlib>


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
Window::Window(const Window&){
    abort();
}

Window&
Window::operator=(const Window&) {
    abort();
    return *this;
}

//
// Protected
//
void
Window::unrealize() {
    UNREALIZE_ENTER;
    if (__widget) __widget->unrealize();
    WindowBase::unrealize();

    // With the introduction of Focus Group IDs, we have to destroy
    // the Focus Group AFTER the Widget has been removed from the
    // Focus Group.
    UNREALIZE_LEAVE;
}

//
// Public
//

Window::Window(const Margin& m): WindowBase(m),
				 __widget(NULL),
				 __fgid((fgid_t)-1) {
    // It is imperative that a new Focus Group is created before the
    // Widget is realized()!
    __fgid = FocusManager::new_focus_group();
}

Window::~Window() {
    FocusManager::destroy_focus_group(__fgid);
    __fgid = (fgid_t)-1;
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
    YAPET::UI::Colors::set_color(curses_window(), YAPET::UI::DEFAULT);
    // Setting background also helps getting rid of artifacts of
    // overlapped windows.
    YAPET::UI::Colors::set_bg(curses_window(), YAPET::UI::DEFAULT);

    WindowBase::refresh(immediate);

    assert(__fgid!=(fgid_t)-1);
    FocusManager::focus_group_activate(__fgid);

    if (__widget) __widget->refresh(immediate);
}

void
Window::realize() {
    REALIZE_ENTER;
    WindowBase::realize();

    FocusManager::focus_group_activate(__fgid);

    if (__widget) {
	assert(__widget->realization()==UNREALIZED);

	// This is imperative, so that the widget also is aware of the
	// new curses window in case we're called in the course of a
	// resize.
	__widget->curses_window(curses_window());

	// This widget does not have another widget as parent.
	__widget->parent(NULL);

	__widget->focusgroup_id(__fgid);

	__widget->position(widget_area());

	__widget->size_available(widget_area());

	__widget->realize();
    }
    REALIZE_LEAVE;
}
