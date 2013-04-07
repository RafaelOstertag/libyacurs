// $Id$

#include <cstdlib>
#include <cassert>

#include "messagebox.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
MessageBox&
MessageBox::operator=(const MessageBox&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//

MessageBox::MessageBox(const std::string& _title,
		       const std::string& _message,
		       DIALOG_TYPE _dt): Dialog(_title, _dt),
					 __message(0) {
    __message=new Label(_message);
    widget(__message);
}

MessageBox::~MessageBox() {
    assert(__message!=0);
    delete __message;
}

