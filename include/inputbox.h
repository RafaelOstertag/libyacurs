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
#define INPUTBOX_H

#include <string>

#include "dialog.h"
#include "label.h"
#include "input.h"

namespace YACURS {
    class InputBox : public Dialog {
        private:
            Label* __message;
            Input<>* __input;
            VPack* __vpack;

            // Not supported
            InputBox& operator=(const InputBox&);

        public:
            InputBox(const std::string& _title,
                     const std::string& _message,
                     DIALOG_TYPE _dt=OKCANCEL);

            virtual ~InputBox();

            const std::string& input() const;

            void clear();
    };
}

#endif // INPUTBOX_H
