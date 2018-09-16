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

#ifndef UNLOCKDIALOG_H
#define UNLOCKDIALOG_H 1

#include <string>

#include "dialog.h"

namespace YACURS {
class UnlockDialog : public Dialog {
   private:
    UnlockDialog& operator=(const UnlockDialog&);

   public:
    UnlockDialog(const std::string& _title);
    virtual ~UnlockDialog();

    // From Dialog
    void realize();

    /**
     * @return @c true if unlock should happen, @c false
     * otherwise.
     */
    virtual bool unlock() = 0;

    virtual void clear() = 0;
};
}  // namespace YACURS

#endif  // UNLOCKDIALOG_H
