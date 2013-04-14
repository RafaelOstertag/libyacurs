//
// This file is part of libyacurs, 
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#include <cstdlib>
#include <cassert>

#include "messagebox2.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
MessageBox2&
MessageBox2::operator=(const MessageBox2&) {
    throw EXCEPTIONS::NotSupported();
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

