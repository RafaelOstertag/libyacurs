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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <cassert>
#include <cerrno>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include "fileloaddialog.h"
#include "yacursconst.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//

//
// Public
//

FileLoadDialog::FileLoadDialog(std::string path, bool do_chdir)
    : FileDialog(std::string(_("Load File")), path, do_chdir, OKCANCEL) {
    filename_readonly(true);
    // Only allow files
    selection_type(YACURS::FILE);
}

FileLoadDialog::~FileLoadDialog() {}
