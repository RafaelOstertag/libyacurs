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

#include "curs.h"
#include "statusbar.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

void StatusBar::show_top_msg() {
    if (_messages.empty())
        line(std::string());
    else
        line(_messages.top());
}

//
// Protected
//

//
// Public
//

StatusBar::StatusBar() : LineObject(POS_BOTTOM, std::string(), STATUSBAR) {}

StatusBar::~StatusBar() {}

void StatusBar::push(const std::string& m) {
    _messages.push(m);
    show_top_msg();
}

void StatusBar::set(const std::string& m) {
    if (!_messages.empty()) _messages.pop();

    _messages.push(m);

    show_top_msg();
}

void StatusBar::pop() {
    if (_messages.empty()) return;
    _messages.pop();
    show_top_msg();
}
