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
    __size(Size(1, __label.length()+1)) {
}

Label::Label(const Label& _l):
    Widget(_l), __label(_l.__label), __size(_l.__size) {
}

Label::~Label() {
}

const Label&
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

    __size = Size(1, __label.length()+1);

    // If parent is NULL, we have nobody to inform about a possible
    // size change. If old size is the same as the new size, we simply
    // have to refresh immediately.
    //
    // In any case, we can leave the function.
    if (parent()==NULL || oldsize == __size) {
	if (realized()) refresh(true);
	return;
    }

    //
    // The size has changed

    if (realized()) {
	// We have to clear the window since the new size might be
	// smaller, and thus leaving artifacts on the screen if we
	// omit to clear the entire subwin()
	if (wclear(subwin()) == ERR)
	    throw ClearFailed();
	if (wrefresh(subwin()) == ERR)
	    throw RefreshFailed();
    }

    // notify parent of size change
    parent()->sizechange(); 
}

const std::string&
Label::label() const {
    return __label;
}

const Size&
Label::size() const {
    return __size;
}

Size
Label::size_hint() const {
    return __size;
}

bool
Label::sizechange() {
    // We don't handle size changes since we're not a container
    // Widget.
    return false;
}

void
Label::resetsize() {
    // Intentionally empty, since resetsize() is intended for dynamically sized
    // Widgets.
}

void
Label::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    assert(subwin()!=NULL);

    if (mymvwaddstr(subwin(), 0, 0,
		    __label.c_str()) == ERR)
	throw AddStrFailed();

    Widget::refresh(immediate);
}
