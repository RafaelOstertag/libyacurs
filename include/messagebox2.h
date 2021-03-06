// -*- mode: c++ -*-
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

#ifndef MESSAGEBOX2_H
#define MESSAGEBOX2_H 1

#include <string>

#include "messagebox.h"

namespace YACURS {
class MessageBox2 : public MessageBox {
   private:
    DynLabel _message2;

   public:
    MessageBox2(const std::string& title, const std::string& message1,
                const std::string& message2, DIALOG_TYPE dt = OKCANCEL);

    MessageBox2& operator=(const MessageBox2&) = delete;
    MessageBox2& operator=(MessageBox2&&) = delete;
    MessageBox2(const MessageBox2&) = delete;
    MessageBox2(MessageBox2&&) = delete;

    virtual ~MessageBox2();
};
}  // namespace YACURS

#endif  // MESSAGEBOX2_H
