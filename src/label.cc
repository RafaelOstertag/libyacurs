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

    if (parent()!=NULL && oldsize != __size)
	parent()->sizechange(); // notify parent of size change
}

const std::string&
Label::label() const {
    return __label;
}

const Size&
Label::size() const {
    return __size;
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
