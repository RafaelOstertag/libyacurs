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

#ifndef YACURSDBG_H
#define YACURSDBG_H 1

#include <fstream>
#include <sstream>

#if defined(DEBUG)
namespace YACURS {
enum DBGTYPE { DBG_EVT, DBG_FOCUSGRP, DBG_FOCUSMGR };

class Debug {
   private:
    static std::ofstream __debugfile;

   public:
    static void out(DBGTYPE dt, const std::string& c);
    static void out(DBGTYPE dt, const std::ostringstream& os);
};
}  // namespace YACURS
#define DEBUGOUT(t, x)                                     \
    {                                                      \
        std::ostringstream __tmp_debug_ostring_stream;     \
        __tmp_debug_ostring_stream << x;                   \
        YACURS::Debug::out(t, __tmp_debug_ostring_stream); \
    }
#else
#define DEBUGOUT(t, x)
#endif

#endif  // YACURSDBG_H
