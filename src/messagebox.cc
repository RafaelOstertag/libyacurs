// $Id$

#include <cstdlib>
#include <cassert>

#include "messagebox.h"

//
// Private
//
MessageBox&
MessageBox::operator=(const MessageBox&) {
    abort();
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
					 __message(NULL) {
    __message=new Label(_message);
    widget(__message);
}

MessageBox::~MessageBox() {
    assert(__message!=NULL);
    delete __message;
}

