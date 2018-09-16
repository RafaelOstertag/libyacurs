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

#include "messagebox3.h"

#include "yacursex.h"

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

MessageBox3::MessageBox3(const std::string& title,
                         const std::string& message1,
                         const std::string& message2,
                         const std::string& message3, DIALOG_TYPE dt)
    : MessageBox2(title, message1, message2, dt), _message3(message3) {
    _message3.color(DIALOG);
    _vpack.add_back(&_message3);
}

MessageBox3::~MessageBox3() {}
