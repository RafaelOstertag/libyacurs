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

#include <cassert>
#include <cstdlib>
#include <stdexcept>

#include "yacursdbg.h"

#if defined(DEBUG)
using namespace YACURS;

std::ofstream Debug::__debugfile;

void
Debug::out(DBGTYPE dt, const std::string& c) {
    try {
	char* __debugfile_name__ = std::getenv("LIBYACURS_DEBUG_LOGFILE");
	if (__debugfile_name__ != 0 ) {
	    if (!__debugfile.is_open() )
		__debugfile.open(__debugfile_name__,
				 std::ios::out | std::ios::trunc);

	    std::string prefix;
	    switch(dt) {
	    case DBG_EVT:
		if (std::getenv("LIBYACURS_EVTQUEUE") == 0)
		    return;
		prefix = "[EVTQUEUE]";
		break;

	    case DBG_FOCUSGRP:
		if (std::getenv("LIBYACURS_FOCUSGRP") == 0)
		    return;
		prefix = "[FOCUSGRP]";
		break;

	    case DBG_FOCUSMGR:
		if (std::getenv("LIBYACURS_FOCUSMGR") == 0)
		    return;
		prefix = "[FOCUSMGR]";
		break;

	    default:
		throw std::invalid_argument("Unknown debug type");
		break;
	    }

	    __debugfile << prefix << " " << c << std::endl;
	}
    } catch (...) {
    }
}

void 
Debug::out(DBGTYPE dt, const std::ostringstream& os) {
    out(dt, os.str());
}
#endif
