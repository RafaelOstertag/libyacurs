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

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H 1

#include <string>

#include "dialog.h"
#include "dynlabel.h"

namespace YACURS {
class MessageBox : public Dialog {
   private:
    DynLabel _message;

   protected:
    VPack _vpack;

   public:
    MessageBox(const std::string& title, const std::string& message,
               DIALOG_TYPE dt = OKCANCEL);

    MessageBox& operator=(const MessageBox&) = delete;
 MessageBox& operator=(MessageBox&&) = delete;
 MessageBox(const MessageBox&) = delete;
 MessageBox(MessageBox&&) = delete;

    virtual ~MessageBox();
};
}  // namespace YACURS

#endif  // MESSAGEBOX_H
