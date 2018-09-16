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

#ifndef FILESAVEDIALOG_H
#define FILESAVEDIALOG_H 1

#include <string>

#include <unistd.h>

#include "dynlabel.h"
#include "filedialog.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

namespace YACURS {
class FileSaveDialog : public FileDialog {
   private:
    MessageBox2* _confirmdia;
    MessageBox2* _errmsgbox;

   protected:
    virtual void window_close_handler(Event& e);

    virtual void button_press_handler(Event& e);

   public:
    FileSaveDialog(std::string path = std::string(), bool do_chdir = false,
                   DIALOG_TYPE dt = OKCANCEL);
    FileSaveDialog& operator=(const FileSaveDialog&) = delete;
    FileSaveDialog& operator=(FileSaveDialog&&) = delete;
    FileSaveDialog(const FileSaveDialog&) = delete;
    FileSaveDialog(FileSaveDialog&&) = delete;

    virtual ~FileSaveDialog();
};
}  // namespace YACURS

#endif  // FILESAVEDIALOG_H
