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

#include "dialog.h"
#include "dynlabel.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

namespace YACURS {
    class FileLoadDialog : public Dialog {
        private:
            MessageBox2* __msgbox;
            DynLabel* __path;
            ListBox<>* __directories;
            ListBox<>* __files;
            Input<>* __filename;
            HPack* __hpack;
            VPack* __vpack;

            // Not supported
            FileLoadDialog& operator=(const FileLoadDialog&);

            std::string dir_up(const std::string& dir);

            void read_dir();

            void listbox_enter_handler(Event& _e);

            void window_close_handler(Event& _e);

        public:
            FileLoadDialog(std::string _path=std::string());

            virtual ~FileLoadDialog();

            std::string filepath() const;

            const std::string& directory() const;

            const std::string& filename() const;

            void refresh(bool immediate);
    };
}

#endif // FILELOADDIALOG_H
