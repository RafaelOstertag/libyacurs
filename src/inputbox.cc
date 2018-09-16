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

#include <cassert>
#include <cstdlib>

#include "inputbox.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//

//
// Public
//

InputBox::InputBox(const std::string& title, const std::string& message,
                   DIALOG_TYPE dt)
    : Dialog(title, dt), _message(message), _input(), _vpack() {
    _message.color(DIALOG);

    _vpack.add_back(&_message);
    _vpack.add_back(&_input);
    _vpack.always_dynamic(true);
    _vpack.hinting(true);

    widget(&_vpack);
}

InputBox::~InputBox() {}

const std::string& InputBox::input() const { return _input.input(); }

void InputBox::clear() { _input.clear(); }
