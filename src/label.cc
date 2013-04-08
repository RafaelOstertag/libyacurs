// $Id$

#include <cassert>
#include <cstdlib>

#include "label.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
Label&
Label::operator=(const Label&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
    
//
// Public
//
Label::Label(const std::string& _l): __color(DEFAULT),
				     __label(_l),
				     __size(Size(1, _l.length())) {
}

Label::~Label() {
}

void
Label::label(const std::string& _l) {
    __label = _l;

    Size oldsize = __size;

    __size = Size(1, __label.length());

    // If parent is 0, we have nobody to inform about a possible
    // size change. If old size is the same as the new size, we simply
    // have to refresh immediately.
    //
    // In any case, we can leave the function.
    if (parent()==0 || oldsize.cols() >= __size.cols()) {
	if (realization()==REALIZED) refresh(true);
	return;
    }
    
    // The size has changed notify parent of size change
    parent()->size_change(); 
}

const std::string&
Label::label() const {
    return __label;
}

Size
Label::size() const {
    return __size;
}

Size
Label::size_hint() const {
    return __size;
}

bool
Label::size_change() {
    // We don't handle size changes since we're not a container
    // Widget.
    return false;
}

void
Label::reset_size() {
    // Intentionally empty, since reset_size() is intended for
    // dynamically sized Widgets.
}

void
Label::refresh(bool immediate) {
    if (realization()!=REALIZED && realization()!=REALIZING) return;

    assert(widget_subwin()!=0);

    widget_subwin()->erase();
    YACURS::INTERNAL::CurStr tmp(__label,Coordinates(), color());
    widget_subwin()->addstr(tmp);

    Widget::refresh(immediate);
}

void
WidgetBase::unrealize() {
    UNREALIZE_ENTER;

    // Intentionally empty

    UNREALIZE_LEAVE;
}

void
Label::color(COLOROBJ c) {
    __color=c;
}

COLOROBJ
Label::color() const {
    return __color;
}
