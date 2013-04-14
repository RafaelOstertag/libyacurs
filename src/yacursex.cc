//
// This file is part of libyacurs, 
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
#include "yacursex.h"

using namespace YACURS::EXCEPTIONS;

BaseCurEx::BaseCurEx(const char* m) : msg(m) {}

BaseCurEx::BaseCurEx(const std::string& m): msg(m) {}

BaseCurEx::~BaseCurEx() throw() {}

const char*
BaseCurEx::what() const throw() { return msg.c_str(); }

CursesException::CursesException(const char* cfct):
    BaseCurEx(std::string(cfct) + std::string(_("() failed"))) {}

UnableToInitialize::UnableToInitialize() :
    BaseCurEx(_("unable to initialize curses")) {}

NotInitialized::NotInitialized() :
    BaseCurEx(_("curses not initialized")) {}

AlreadyInitialized::AlreadyInitialized() :
    BaseCurEx(_("curses has already been initialized")) {}

AlreadyRealized::AlreadyRealized():
    BaseCurEx(_("object already realized")) {}

NotRealized::NotRealized():
    BaseCurEx(_("object not realized")) {}

SystemError::SystemError(int _errno):
    BaseCurEx(std::strerror(_errno)),
    __errno(_errno) {}

int
SystemError::errorno() const { return __errno; }

WinSizeInvalid::WinSizeInvalid():
    BaseCurEx(_("TIOCGWINSZ info invalid")) {}

UnableToGetWinSize::UnableToGetWinSize():
    BaseCurEx(_("unable to get window size")) {}

CannotFocus::CannotFocus():
    BaseCurEx(_("Cannot focus")) {}

UnexpectedEvent::UnexpectedEvent():
    BaseCurEx(_("Unexpected event received")) {}

ColorsNotInitialized::ColorsNotInitialized():
    BaseCurEx(_("Colors not initialized")) {}

AreaExceeded::AreaExceeded():
    BaseCurEx(_("An object exceeded space constraints")) {}

NotSupported::NotSupported():
    BaseCurEx(_("Operation not supported")) {}

InvalidDialogType::InvalidDialogType():
    BaseCurEx(_("Invalid dialog type")) {}
