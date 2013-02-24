// $Id$

#include <cassert>
#include <cstdlib>

#include "label.h"
#include "cursex.h"

//
// Private
//
Label&
Label::operator=(const Label&) {
    abort();
    return *this;
}

//
// Protected
//
    
//
// Public
//
Label::Label(const std::string& _l): Widget(),
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

    // If parent is NULL, we have nobody to inform about a possible
    // size change. If old size is the same as the new size, we simply
    // have to refresh immediately.
    //
    // In any case, we can leave the function.
    if (parent()==NULL || oldsize.cols() >= __size.cols()) {
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
