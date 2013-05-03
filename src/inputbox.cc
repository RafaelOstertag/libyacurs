//
// This file is part of libyacurs.
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

#include "inputbox.h"

using namespace YACURS;

//
// Private
//
InputBox&
InputBox::operator=(const InputBox&) {
    throw EXCEPTIONS::NotSupported();
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
                   DIALOG_TYPE _dt) : Dialog(_title, _dt),
    __message(0),
    __input(0),
    __vpack(0) {
    __message = new Label(_message);
    __message->color(DIALOG);
    __input = new Input<>;
    __vpack = new VPack;

    __vpack->add_back(__message);
    __vpack->add_back(__input);
    __vpack->always_dynamic(true);
    __vpack->hinting(true);

    widget(__vpack);
}

InputBox::~InputBox() {
    assert(__message != 0);
    assert(__input != 0);
    assert(__vpack != 0);

    delete __input;
    delete __message;
    delete __vpack;
}

const std::string&
InputBox::input() const {
    assert(__input != 0);

    return __input->input();
}

void
InputBox::clear() {
    assert(__input != 0);
    __input->clear();
}
