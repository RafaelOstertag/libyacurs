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

#ifndef UNLOCKDIADEFAULT_H
#define UNLOCKDIADEFAULT_H

#include <string>

#include "unlockdialog.h"
#include "vpack.h"
#include "label.h"
#include "input.h"

namespace YACURS {
    /**
     * Default unlock dialog.
     *
     * Convenience class providing simple unlock dialog.
     */
    class UnlockDialogDefault : public UnlockDialog {
        private:
            std::string __secret;
            VPack* __vpack;
            Label* __text;
            Input<>* __secret_input;

            UnlockDialogDefault& operator=(const UnlockDialogDefault&);

        public:
            UnlockDialogDefault(const std::string& _secret);

            ~UnlockDialogDefault();

            bool unlock();

            void clear();
    };
}

#endif // UNLOCKDIADEFAULT_H
