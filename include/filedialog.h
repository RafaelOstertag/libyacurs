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

#ifndef FILEDIALOG_H
#define FILEDIALOG_H 1

#include <string>

#include <unistd.h>

#include "dialog.h"
#include "dynlabel.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

namespace YACURS {
class FileDialog : public Dialog {
   private:
    MessageBox2* _errmsgbox;
    MessageBox2* _filetypemismatch;
    DynLabel _path;
    ListBox<> _directories;
    ListBox<> _files;
    Input<> _filename;
    HPack _hpack;
    VPack _vpack;
    bool _do_chdir;
    FILEDIALOG_SELECTION_TYPE _sel_type;
    std::string _suffix;

   std::string cleanup_path(const std::string& p) const;

   protected:
    std::string dir_up(const std::string& dir);

    void read_dir();

    void listbox_enter_handler(Event& e);

    void filename_readonly(bool ro);

    std::string getcwd() const;

    virtual void window_close_handler(Event& e);

    virtual void button_press_handler(Event& e);

    /**
     * Test if the selected file path matches the requested
     * selection type.
     *
     * Be advised, if the method returns @c false, it has
     * created a dialog.
     *
     * @return @c true if the selected file matches the
     * requested selection type. @c false otherwise.
     */
    bool selection_type_match();

   public:
    FileDialog(const std::string& title, std::string path = std::string(),
               bool do_chdir = false, DIALOG_TYPE _dt = OKCANCEL);

    FileDialog& operator=(const FileDialog&) = delete;
    FileDialog& operator=(FileDialog&&) = delete;
    FileDialog(const FileDialog&) = delete;
    FileDialog(FileDialog&&) = delete;
    virtual ~FileDialog();

    std::string filepath() const;

    const std::string& directory() const;

    std::string filename() const;

    void do_chdir(bool v);

    bool do_chdir() const;

    void selection_type(FILEDIALOG_SELECTION_TYPE t);

    FILEDIALOG_SELECTION_TYPE selection_type() const;

    /**
     * File name suffix.
     *
     * String that is appended to file name and file path when
     * selection type is FILE.
     */
    void suffix(const std::string& s);

    void refresh(bool immediate);
};
}  // namespace YACURS

#endif  // FILEDIALOG_H
