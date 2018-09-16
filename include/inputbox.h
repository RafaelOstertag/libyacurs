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

#ifndef INPUTBOX_H
#define INPUTBOX_H 1

#include <string>

#include "dialog.h"
#include "input.h"
#include "label.h"

namespace YACURS {
class InputBox : public Dialog {
   private:
    Label _message;
    Input<> _input;
    VPack _vpack;

   public:
    InputBox(const std::string& title, const std::string& message,
             DIALOG_TYPE dt = OKCANCEL);

    InputBox& operator=(const InputBox&) = delete;
    InputBox& operator=(InputBox&&) = delete;
    InputBox(const InputBox&) = delete;
    InputBox(InputBox&&) = delete;
    virtual ~InputBox();

    const std::string& input() const;

    void clear();
};
}  // namespace YACURS

#endif  // INPUTBOX_H
