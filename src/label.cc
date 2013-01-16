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
    Widget(), ScreenObject(), __label(_l) {
    __size = Rectangle(0, 0, 1, __label.length());
}

Label::Label(const Label& _l):
    Widget(_l), ScreenObject(_l), __label(_l.__label) {
}

Label::~Label() {
}

const Label&
Label::operator=(const Label& _l) {
    Widget::operator=(_l);
    ScreenObject::operator=(_l);

    __label = _l.__label;

    return *this;
}

Widget*
Label::clone() const {
    return new Label(*this);
}

void
Label::label(const std::string& _l) {
    __label = _l;
    __size = Rectangle(0, 0, 1, __label.length());
}

const std::string&
Label::label() const {
    return __label;
}
