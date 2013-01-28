// $Id$

#include <cassert>

#include "label.h"
#include "cursex.h"

//
// Private
//

//
// Protected
//
    
//
// Public
//
Label::Label(const std::string& _l):
    Widget(), __label(_l),
    __size(Size(1, _l.length())) {
}

Label::Label(const Label& _l):
    Widget(_l), __label(_l.__label), __size(_l.__size) {
}

Label::~Label() {
}

Label&
Label::operator=(const Label& _l) {
    Widget::operator=(_l);

    __label = _l.__label;
    __size = _l.__size;

    return *this;
}

void
Label::label(const std::string& _l) {
    __label = _l;

    Size oldsize = __size;

    __size = Size(1, __label.length());

    // If parent is NULL, we have nobody to inform about a possible
    // size change. If old size is the same as the new size, we simply
    // have to refresh immediately.
    //
    // In any case, we can leave the function.
    if (parent()==NULL || oldsize.cols() >= __size.cols()) {
	if (realized()) refresh(true);
	return;
    }
    
    // The size has changed notify parent of size change
    parent()->size_change(); 
}

std::string
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

bool
Label::can_focus() const {
    return false;
}

void
Label::focus(bool) {
    throw CannotFocus();
}

bool 
Label::focus() const {
    return false;
}

void
Label::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    assert(widget_subwin()!=NULL);

    // We don't resize if a new string is set that was smaller than
    // the previous one. This leads to artifacts, so do a werase()
    // first.
    if (werase(widget_subwin())==ERR)
	throw EraseFailed();

    // if (mymvwaddstr(widget_subwin(), 0, 0,
    // 		    __label.c_str()) == ERR)
    // 	throw AddStrFailed();
    //
    // We ignore the error returned, since the cursor cannot be
    // advanced past the end, and thus the string is
    // truncated. However, the truncation has no effect on label.
    mymvwaddstr(widget_subwin(), 0, 0,  __label.c_str());

    Widget::refresh(immediate);
}
