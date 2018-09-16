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

#ifndef FILELOADDIALOG_H
#define FILELOADDIALOG_H 1

#include <string>

#include <unistd.h>

#include "dynlabel.h"
#include "filedialog.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

namespace YACURS {
class FileLoadDialog : public FileDialog {
   private:
    // Not supported
    FileLoadDialog& operator=(const FileLoadDialog&);

   public:
    FileLoadDialog(std::string _path = std::string(), bool _do_chdir = false);

    virtual ~FileLoadDialog();
};
}  // namespace YACURS

#endif  // FILELOADDIALOG_H
