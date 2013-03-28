// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>

#include "widgetbase.h"
#include "focusmanager.h"
#include "cursex.h"

//
// Private
//

WidgetBase::WidgetBase(const WidgetBase&){
    throw NotSupported();
}

WidgetBase&
WidgetBase::operator=(const WidgetBase&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
WINDOW*
WidgetBase::curses_window() const {
    return __curses_window;
}

fgid_t
WidgetBase::focusgroup_id() const {
    return __fgid;
}

WidgetBase*
WidgetBase::parent() const {
    return __parent;
}

void
WidgetBase::can_focus(bool _can_focus) {
    __can_focus=_can_focus;
}

//
// Private
//

WidgetBase::WidgetBase(): __curses_window(0),
			  __fgid((fgid_t)-1),
			  __can_focus(false),
			  __focus(false),
			  __parent(0) {
}

WidgetBase::~WidgetBase() {
    if (__can_focus && __fgid!=(fgid_t)-1)
	FocusManager::focus_group_remove(__fgid, this);
}

void
WidgetBase::parent(WidgetBase* _p) {
    __parent = _p;
}

void
WidgetBase::curses_window(WINDOW* _p) {
    __curses_window=_p;
}

void
WidgetBase::focusgroup_id(fgid_t _id) {
    // Only make changes to the Focus Group if Focus Group ID differs.
    //
    // This ensures that the currently focused Widget does not loose
    // the focus when resizing.
    if (__can_focus && __fgid!=_id) {
	// Remove the widget from the current focus group, if possible
	if (__fgid!=(fgid_t)-1)
	    FocusManager::focus_group_remove(__fgid, this);

	// Add the widget to the new focus group if possible
	if (_id!=(fgid_t)-1)
	    FocusManager::focus_group_add(_id, this);
    }
    __fgid=_id;
}

void
WidgetBase::position(const Coordinates& _c) {
    assert(_c.x()>-1);
    assert(_c.y()>-1);
    __position = _c;
}

const Coordinates&
WidgetBase::position() const {
    return __position;
}

void
WidgetBase::size_available(const Size& _s) {
    assert(_s.rows()>0);
    assert(_s.cols()>0);
    __size_available = _s;
}

const Size&
WidgetBase::size_available() const {
    return __size_available;
}

bool
WidgetBase::can_focus() const {
    return __can_focus;
}

void
WidgetBase::focus(bool _f) {
    if (!__can_focus)
	throw CannotFocus();

    __focus=_f;
}

bool
WidgetBase::focus() const {
    if (!__can_focus)
	throw CannotFocus();

    return __focus;
}

void
WidgetBase::unrealize() {
    UNREALIZE_ENTER;

    // Intentionally empty

    UNREALIZE_LEAVE;
}
