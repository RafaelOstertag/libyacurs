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

#ifndef YACURSTYPES_H
#define YACURSTYPES_H 1

#include <signal.h>

namespace YACURS {
// Forward declaration because we want to be independent.
class Event;

/// Type of the function pointer that will be called upon an
/// event.
typedef void (*fptr_t)(Event&);

enum SORT_ORDER { ASCENDING, DESCENDING, UNSORTED };

/**
 * State of dialog.
 */
enum DIALOG_STATE {
    /// Dialog was confirmed
    DIALOG_OK,
    DIALOG_YES,
    /// Dialog was cancelled
    DIALOG_CANCEL,
    DIALOG_NO
};

enum DIALOG_TYPE { OK_ONLY, YES_ONLY, OKCANCEL, YESNO, YESNOCANCEL };

enum DIALOG_SIZE { AUTOMATIC, FULLSIZE };

/**
 * File types allowed for selection in FileDialog.
 */
enum FILEDIALOG_SELECTION_TYPE { ANY, DIRECTORY, FILE };

namespace INTERNAL {
#ifdef SA_SIGINFO

typedef void (*sig_handler)(int, siginfo_t*, void*);
#else

typedef void (*sig_handler)(int);
#endif
}  // namespace INTERNAL
}  // namespace YACURS

#endif  // YACURSTYPES_H
