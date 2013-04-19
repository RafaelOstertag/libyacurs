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

#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <string>

#include "window.h"
#include "unlockdialog.h"
#include "messagebox.h"

namespace YACURS {
    class LockScreen : public Window {
        private:
            unsigned int __timeout;
            unsigned int __unlock_diag_timeout;
            UnlockDialog* __unlock_dialog;
            MessageBox* __msgbox;
            // Not supported
            LockScreen& operator=(const LockScreen&);

        protected:
            virtual void key_event_handler(Event& _e);

            virtual void window_close_event_handler(Event& _e);

        public:
            LockScreen(UnlockDialog* _unlock,
                       unsigned int timeout,
                       unsigned int ulck_timeout);
            virtual ~LockScreen();

            unsigned int timeout() const;

            unsigned int unlock_dialog_timeout() const;

            virtual void close_unlock_dialog();

            // From WindowBase
            void show();

            void close();
    };
}

#endif // LOCKSCREEN_H
