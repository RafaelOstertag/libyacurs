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
#define UNLOCKDIADEFAULT_H 1

#include <string>

#include "input.h"
#include "label.h"
#include "unlockdialog.h"
#include "vpack.h"

namespace YACURS {
/**
 * Default unlock dialog.
 *
 * Convenience class providing simple unlock dialog.
 */
class UnlockDialogDefault : public UnlockDialog {
   private:
    std::string _secret;
    VPack _vpack;
    Label _text;
    Input<> _secret_input;

   public:
    UnlockDialogDefault(const std::string& secret);
    UnlockDialogDefault& operator=(const UnlockDialogDefault&) = delete;
    UnlockDialogDefault(const UnlockDialogDefault&) = delete;
    UnlockDialogDefault& operator=(UnlockDialogDefault&&) = delete;
    UnlockDialogDefault(UnlockDialogDefault&&) = delete;
    ~UnlockDialogDefault();

    bool unlock();

    void clear();
};
}  // namespace YACURS

#endif  // UNLOCKDIADEFAULT_H
