// -*- mode: c++ -*-
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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <stack>

#include "lineobject.h"

namespace YACURS {
    /**
     * Status Line.
     *
     * Maintains a stack of messages, where the top most message will
     * be displayed.
     */
    class StatusBar : public LineObject {
        private:
            std::stack<std::string> __messages;

            void put_top_msg();

            StatusBar& operator=(const StatusBar&);

        public:
            StatusBar();
            virtual ~StatusBar();
            void push_msg(const std::string& m);

            void pop_msg();
    };
}

#endif
