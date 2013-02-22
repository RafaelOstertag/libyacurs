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
    abort();
}

WidgetBase&
WidgetBase::operator=(const WidgetBase&) {
    abort();
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

//
// Private
//

WidgetBase::WidgetBase(): Realizeable(),
			  __curses_window(NULL),
			  __fgid((fgid_t)-1),
			  __parent(NULL),
			  __position(),
			  __size_available() {
}

WidgetBase::~WidgetBase() {
    // Make sure the widget is removed from the focus group.
    //
    // we don't call can_focus(), since that might already been
    // destroyed.
    if (__fgid!=(fgid_t)-1)
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
    if (can_focus()) {
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

void
WidgetBase::unrealize() {
    UNREALIZE_ENTER;

    __fgid=(fgid_t)-1;

    UNREALIZE_LEAVE;
}
