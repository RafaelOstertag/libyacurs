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

#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H 1

#include <string>

#include "messagebox.h"
#include "unlockdialog.h"
#include "window.h"

namespace YACURS {
class LockScreen : public Window {
   private:
    unsigned int _timeout;
    unsigned int _unlock_diag_timeout;
    UnlockDialog* _unlock_dialog;
    MessageBox* _msgbox;

   protected:
    virtual void key_event_handler(Event& e);

    virtual void window_close_event_handler(Event& e);

   public:
    LockScreen(UnlockDialog* unlock, unsigned int timeout,
               unsigned int ulck_timeout);
    LockScreen& operator=(const LockScreen&) = delete;
    LockScreen& operator=(LockScreen&&) = delete;
    LockScreen(const LockScreen&) = delete;
    LockScreen(LockScreen&&) = delete;
    virtual ~LockScreen();

    unsigned int timeout() const;

    unsigned int unlock_dialog_timeout() const;

    virtual void close_unlock_dialog();

    // From WindowBase
    void show();

    void close();
};
}  // namespace YACURS

#endif  // LOCKSCREEN_H
