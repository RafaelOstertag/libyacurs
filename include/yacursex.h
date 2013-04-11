/* -*- mode: c++ -*-
 *
 * This file is part of libyacurs, 
 * Copyright (C) 2013  Rafael Ostertag
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

// $Id$

#ifndef CUREX_H
#define CUREX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstring>
#include <exception>
#include <string>

namespace YACURS {
    /**
     * Base class from which all libyacurs exceptions are derived.
     */
    class BaseCurEx: public std::exception {
	private:
	    /**
	     * The message of the exception.
	     */
	    std::string msg;

	public:
	    BaseCurEx(const char* m);
	    BaseCurEx(const std::string& m);
	    virtual ~BaseCurEx() throw();

	    /**
	     * Show the error message.
	     *
	     * @return error message.
	     */
	    const char* what() const throw();
    };

    class CursesException: public BaseCurEx {
	public:
	    CursesException(const char* cfct);
    };

    class UnableToInitialize: public BaseCurEx {
	public:
	    UnableToInitialize();
    };

    class NotInitialized: public BaseCurEx {
	public:
	    NotInitialized();
    };

    class AlreadyInitialized: public BaseCurEx {
	public:
	    AlreadyInitialized();
    };

    class AlreadyRealized: public BaseCurEx {
	public:
	    AlreadyRealized();
    };

    class NotRealized: public BaseCurEx {
	public:
	    NotRealized();
    };

    class SystemError: public BaseCurEx {
	private:
	    int __errno;

	public:
	    SystemError(int _errno);
	    int errorno() const;
    };

    class WinSizeInvalid: public BaseCurEx {
	public:
	    WinSizeInvalid();
    };

    class UnableToGetWinSize: public BaseCurEx {
	public:
	    UnableToGetWinSize();
    };

    class CannotFocus: public BaseCurEx {
	public:
	    CannotFocus();
    };

    class UnexpectedEvent: public BaseCurEx {
	public:
	    UnexpectedEvent();
    };

    class ColorsNotInitialized: public BaseCurEx {
	public:
	    ColorsNotInitialized();
    };

    class AreaExceeded: public BaseCurEx {
	public:
	    AreaExceeded();
    };

    class NotSupported: public BaseCurEx {
	public:
	    NotSupported();
    };

    class InvalidDialogType: public BaseCurEx {
	public:
	    InvalidDialogType();
    };
}

#endif // CUREX_H
