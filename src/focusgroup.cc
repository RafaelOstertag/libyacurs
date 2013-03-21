// $Id$

#include <cassert>
#include <algorithm>
#include <functional>

#include "focusgroup.h"

//
// Functors
//
#ifndef NDEBUG
/**
 * Class for debugging.
 *
 * Functor counting the number of widgets having focus in the Focus
 * Group.
 *
 * Usually, this should either be 1 or 0.
 */
class CountFocus {
    private:
	int __count;
    public:
	CountFocus(): __count(0) {}
	void operator()(const WidgetBase* _w) {
	    if (_w->focus()) __count++;
	}
	int count() const { return __count; }
};
#endif

//
// Private
//

//
// Protected
//

//
// Public
//

FocusGroup::FocusGroup(): __active(false), __focus(__widgets.end()) {}

FocusGroup::FocusGroup(const FocusGroup& _f): __active(_f.__active),
					      __widgets(_f.__widgets),
					      __focus(_f.__focus) {}

FocusGroup::~FocusGroup() {
    if (__widgets.empty()) return;

    assert(__focus!=__widgets.end());
    assert((*__focus) != 0);
    (*__focus)->focus(false);
}

FocusGroup&
FocusGroup::operator=(const FocusGroup& _f) {
    __active = _f.__active;
    __widgets = _f.__widgets;
    __focus = _f.__focus;
    return *this;
}

void
FocusGroup::activate() {
    if (__active) return;

   __active=true;

    if (__widgets.empty()) return;

    // If we are called the first time, make the first Widget getting
    // the focus. Else do not change __focus, allowing to resume the
    // last state of the focus in the group.
    if (__focus == __widgets.end())
	__focus = __widgets.begin();

    assert((*__focus)!=0);

    (*__focus)->focus(true);

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    CountFocus cf=std::for_each(__widgets.begin(),
				__widgets.end(),
				CountFocus());
    assert(cf.count()<2);
#endif // NDEBUG
}

void
FocusGroup::deactivate() {
    if (!__active) return;

    __active=false;

    if (__widgets.empty()) return;

    assert(__focus!=__widgets.end());
    assert((*__focus)!=0);

    (*__focus)->focus(false);
#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    CountFocus cf=std::for_each(__widgets.begin(),
				__widgets.end(),
				CountFocus());
    assert(cf.count()==0);
#endif // NDEBUG
}

bool
FocusGroup::active() const {
    return __active;
}

void
FocusGroup::add(WidgetBase* _w) {
    assert(_w!=0);

    // If the Focus Groups is empty but activate, we set the focus to
    // the first Widget added, so that we don't end up with an
    // activated Focus Group where no widget has the focus.
    //
    // In any other case, activate() will take (has taken) care of
    // giving the focus to a widget.
    if (__widgets.empty()) {
	__widgets.push_back(_w);
	__focus=__widgets.begin();
	(*__focus)->focus(true);
#ifndef NDEBUG
	// Solaris Studio 12.3 forced me to do it that way, i.e. with
	// functor.
	CountFocus cf=std::for_each(__widgets.begin(),
				    __widgets.end(),
				    CountFocus());
	assert(cf.count()==1);
#endif // NDEBUG
    } else {
	__widgets.push_back(_w);
    }
}

void
FocusGroup::remove(WidgetBase* _w) {
    assert(_w!=0);
    assert(!__widgets.empty());

    // Will be set to true, if the removed widget had the focus
    bool removed_had_focus=false;

    // Find out, whether the Widget to be removed has the focus, if
    // so, give the focus to another widget.
    if ((*__focus)==_w) {
	_w->focus(false);

	removed_had_focus=true;

	if (++__focus==__widgets.end())
	    __focus=__widgets.begin();
    }
    
    __widgets.remove(_w);

    // No more widgets left, we cannot assign focus to anything.
    if (__widgets.empty()) return;

    // Make sure we don't point at the end of the list
    if (removed_had_focus)
	(*__focus)->focus(true);

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    CountFocus cf=std::for_each(__widgets.begin(),
				__widgets.end(),
				CountFocus());
    assert(cf.count()<2);
#endif // NDEBUG
}

void
FocusGroup::focus_next() {
    assert(!__widgets.empty());
    if (!__active) return;

    // If activate() has been called, __focus must point to something,
    // but surely not the end().
    assert(__focus != __widgets.end());

    // Not that it would be an error calling focus(false) on a Widget
    // not having the focus, but it would show that something within
    // the FocusGroup is weird.
    assert((*__focus)->focus());

    // remove focus of current Widget.
    (*__focus)->focus(false);
    (*__focus)->refresh(true);

    // Then, advance to the next widget. If we hit the end, we start
    // at the beginning again.
    if ( ++__focus == __widgets.end())
	__focus = __widgets.begin();

    (*__focus)->focus(true);
    (*__focus)->refresh(true);

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    CountFocus cf=std::for_each(__widgets.begin(),
				__widgets.end(),
				CountFocus());
    assert(cf.count()==1);
#endif // NDEBUG
}

void
FocusGroup::focus_previous() {
    assert(!__widgets.empty());
    if (!__active) return;

    // If activate() has been called, __focus must point to something,
    // but surely not the end().
    assert(__focus != __widgets.end());

    // Not that it would be an error calling focus(false) on a Widget
    // not having the focus, but it would show that something within
    // the FocusGroup is weird.
    assert((*__focus)->focus());

    // remove focus of current Widget.
    (*__focus)->focus(false);
    (*__focus)->refresh(true);

    // Then, advance to the previous widget. If we are already at the
    // start, wrap to the last widget
    if (__focus == __widgets.begin())
	__focus = __widgets.end();

    __focus--;

    (*__focus)->focus(true);
    (*__focus)->refresh(true);

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    CountFocus cf=std::for_each(__widgets.begin(),
				__widgets.end(),
				CountFocus());
    assert(cf.count()==1);
#endif // NDEBUG
}    

void
FocusGroup::refocus() const {
    if (__widgets.empty()) return;

    (*__focus)->focus(true);
    (*__focus)->refresh(true);
}
