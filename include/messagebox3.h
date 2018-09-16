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

#ifndef MESSAGEBOX3_H
#define MESSAGEBOX3_H 1

#include "messagebox2.h"

namespace YACURS {
class MessageBox3 : public MessageBox2 {
   private:
    DynLabel _message3;

   public:
    MessageBox3(const std::string& title, const std::string& message1,
                const std::string& message2, const std::string& message3,
                DIALOG_TYPE dt = OKCANCEL);

    MessageBox3& operator=(const MessageBox3&) = delete;
    MessageBox3& operator=(MessageBox3&&) = delete;
    MessageBox3(const MessageBox3&) = delete;
    MessageBox3(MessageBox3&&) = delete;

    virtual ~MessageBox3();
};
}  // namespace YACURS

#endif  // MESSAGEBOX3_H
