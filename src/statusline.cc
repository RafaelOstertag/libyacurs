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

#include "curs.h"
#include "yacursex.h"
#include "statusline.h"

using namespace YACURS;

//
// Private
//

void
StatusLine::put_top_msg() {
    if (__messages.empty() )
        line(std::string() );
    else
        line(__messages.top() );
}

StatusLine&
StatusLine::operator=(const StatusLine&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//

StatusLine::StatusLine() :
    LineObject(POS_BOTTOM) {
}

StatusLine::~StatusLine() {
}

void
StatusLine::push_msg(const std::string& m) {
    __messages.push(m);
    put_top_msg();
}

void
StatusLine::pop_msg() {
    if (__messages.empty() ) return;
    __messages.pop();
    put_top_msg();
}
