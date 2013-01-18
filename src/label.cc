// $Id$

#include "label.h"

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
    Widget(), __label(_l) {
    __size = Area(0, 0, 1, __label.length());
}

Label::Label(const Label& _l):
    Widget(_l), __label(_l.__label) {
}

Label::~Label() {
}

const Label&
Label::operator=(const Label& _l) {
    Widget::operator=(_l);

    __label = _l.__label;

    return *this;
}

void
Label::label(const std::string& _l) {
    __label = _l;
    __size = Area(0, 0, 1, __label.length());
}

const std::string&
Label::label() const {
    return __label;
}
