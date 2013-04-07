// $Id$

#include <cstdlib>
#include <cassert>

#include "inputbox.h"

using namespace YACURS;

//
// Private
//
InputBox&
InputBox::operator=(const InputBox&) {
    throw NotSupported();
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
				     __message(0),
				     __input(0),
				     __vpack(0) {
    __message=new Label(_message);
    __input=new Input<>;
    __vpack=new VPack;

    __vpack->add_back(__message);
    __vpack->add_back(__input);
    __vpack->always_dynamic(true);
    __vpack->hinting(true);

    widget(__vpack);
}

InputBox::~InputBox() {
    assert(__message!=0);
    assert(__input!=0);
    assert(__vpack!=0);

    delete __input;
    delete __message;
    delete __vpack;
}

const std::string&
InputBox::input() const {
    assert(__input!=0);

    return __input->input();
}

void
InputBox::clear() {
    assert(__input!=0);
    __input->clear();
}
