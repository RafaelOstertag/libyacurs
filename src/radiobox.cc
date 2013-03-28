// $Id: input.cc 4875 2013-02-23 13:25:34Z rafisol $

#include <cassert>
#include <cstdlib>

#include "radiobox.h"
#include "cursex.h"

//
// Private
//

RadioBox&
RadioBox::operator=(const RadioBox&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
void
RadioBox::set_selection(unsigned short _cursor) {
    assert(_cursor<__items.size());
    for (std::vector<Selectable>::size_type i=0; i<__items.size(); i++)
	__items[i].selected=false;

    __items[_cursor].selected=true;
}

//
// Public
//

RadioBox::RadioBox(const std::string& _title,
		   const std::vector<std::string>& _items):
    CheckBox(_title, _items) {
    __indicators[0]= "( ) ";
    __indicators[1]= "(o) ";
}

RadioBox::~RadioBox() {
}

