// $Id$

#include <cstdlib>
#include <cassert>

#include "messagebox2.h"
#include "cursex.h"

//
// Private
//
MessageBox2&
MessageBox2::operator=(const MessageBox2&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//

MessageBox2::MessageBox2(const std::string& _title,
			 const std::string& _message1,
			 const std::string& _message2,
			 DIALOG_TYPE _dt): Dialog(_title, _dt),
					   __vpack(0),
					   __message1(0),
					   __message2(0) {
    __message1=new Label(_message1);
    __message2=new Label(_message2);

    __vpack=new VPack;

    __vpack->add_back(__message1);
    __vpack->add_back(__message2);

    widget(__vpack);
}

MessageBox2::~MessageBox2() {
    assert(__vpack!=0);
    assert(__message1!=0);
    assert(__message2!=0);

    delete __vpack;
    delete __message1;
    delete __message2;
}

