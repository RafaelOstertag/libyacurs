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

#ifndef YACURSEX_H
#define YACURSEX_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstring>
#include <exception>
#include <string>

namespace YACURS {
    /**
     * Name space for exceptions.
     */
    namespace EXCEPTIONS {
        /**
         * Base class from which all libyacurs exceptions are derived.
         */
        class BaseCurEx : public std::exception {
            private:
                /**
                 * The message of the exception.
                 */
                std::string msg;

            public:
                /**
                 * Constructor.
                 *
                 * @param m message
                 */
                BaseCurEx(const char* m);

                /**
                 * Copy Constructor.
                 */
                BaseCurEx(const std::string& m);
                virtual ~BaseCurEx() throw();

                /**
                 * Show the error message.
                 *
                 * @return error message.
                 */
                const char* what() const throw();
        };

        /**
         * Curses Exception.
         *
         * Error in Curses function encountered.
         */
        class CursesException : public BaseCurEx {
            public:
                /**
                 * @param cfct name of the Curses function, e.g. @c
                 * waddch, or @c wmvwaddstr, etc.
                 */
                CursesException(const char* cfct);
        };

        /**
         * Initialization failed.
         */
        class UnableToInitialize : public BaseCurEx {
            public:
                UnableToInitialize();
        };

        /**
         * Function called on uninitialized object.
         */
        class NotInitialized : public BaseCurEx {
            public:
                NotInitialized();
        };

        /**
         * Initialization function called on already initialized object.
         */
        class AlreadyInitialized : public BaseCurEx {
            public:
                AlreadyInitialized();
        };

        /**
         * Object is already initialized.
         */
        class AlreadyRealized : public BaseCurEx {
            public:
                AlreadyRealized();
        };

        /**
         * Object is not realized.
         *
         * The object has not been realized.
         */
        class NotRealized : public BaseCurEx {
            public:
                NotRealized();
        };

        /**
         * System Error
         */
        class SystemError : public BaseCurEx {
            private:
                /**
                 * Error number
                 */
                int __errno;

            public:
                /**
                 * @param _errno error number
                 */
                SystemError(int _errno);

                /**
                 * @return error number of the System Error.
                 */
                int errorno() const;
        };

        /**
         * TIOCGWINSZ returned by ioctl() is invalid.
         */
        class WinSizeInvalid : public BaseCurEx {
            public:
                WinSizeInvalid();
        };

        /**
         * Unable to determine the window size.
         */
        class UnableToGetWinSize : public BaseCurEx {
            public:
                UnableToGetWinSize();
        };

        /**
         * Size is invalid.
         */
        class InvalidSize : public BaseCurEx {
            public:
                InvalidSize();
        };

        /**
         * Object cannot be focused.
         */
        class CannotFocus : public BaseCurEx {
            public:
                CannotFocus();
        };

        /**
         * Event not expected.
         *
         * Event received is not expected.
         */
        class UnexpectedEvent : public BaseCurEx {
            public:
                UnexpectedEvent();
        };

        /**
         * Color function called requiring Colors object to be
         * initialized.
         */
        class ColorsNotInitialized : public BaseCurEx {
            public:
                ColorsNotInitialized();
        };

        /**
         * A widget exceeds space constraints.
         */
        class AreaExceeded : public BaseCurEx {
            public:
                AreaExceeded();
        };

        /**
         * Operation not supported.
         */
        class NotSupported : public BaseCurEx {
            public:
                NotSupported();
        };

        /**
         * Dialog type not known.
         */
        class InvalidDialogType : public BaseCurEx {
            public:
                InvalidDialogType();
        };
    } // namespace EXCEPTION
} // namespace YACURS

#endif // YACURSEX_H
