// $Id$

#include <cstdlib>
#include <cassert>

#include "inputbox.h"

//
// Private
//
InputBox&
InputBox::operator=(const InputBox&) {
    abort();
    return *this;
}

//
// Protected
//

//
// Public
//

InputBox::InputBox(const std::string& _title,
		   const std::string& _message,
		   DIALOG_TYPE _dt): Dialog(_title, _dt),
				     __message(NULL),
				     __input(NULL),
				     __vpack(NULL) {
    __message=new Label(_message);
    __input=new Input;
    __vpack=new VPack;

    __vpack->add_back(__message);
    __vpack->add_back(__input);
    __vpack->always_dynamic(true);
    __vpack->hinting(true);

    widget(__vpack);
}

InputBox::~InputBox() {
    assert(__message!=NULL);
    assert(__input!=NULL);
    assert(__vpack!=NULL);

    delete __input;
    delete __message;
    delete __vpack;
}

const std::string&
InputBox::input() const {
    assert(__input!=NULL);

    return __input->input();
}

void
InputBox::clear() {
    assert(__input!=NULL);
    __input->clear();
}
