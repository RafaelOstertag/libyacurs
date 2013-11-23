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
// $Id: area.h 5021 2013-04-12 07:17:47Z rafisol $

#include <cerrno>
#include <cstring>

#include "yacursex.h"
#include "sigaction.h"

using namespace YACURS::INTERNAL;

//
// Private
//
Sigaction::Sigaction(const Sigaction&) {
    throw EXCEPTIONS::NotSupported();
}

Sigaction&
Sigaction::operator=(const Sigaction&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//

Sigaction::Sigaction(int signo, sig_handler hndlr, sigset_t& mask) : __signo(
        signo) {
#ifdef SA_SIGINFO
    __current_action.sa_sigaction = hndlr;
    __current_action.sa_flags = SA_SIGINFO;
#else
    __current_action.sa_handler = hndlr;
    __current_action.sa_flags = 0;
#endif

    std::memcpy(&__current_action.sa_mask, &mask, sizeof (sigset_t) );

    if (sigaction(__signo, &__current_action, &__saved_action) != 0)
        throw EXCEPTIONS::SystemError(errno);
}

Sigaction::~Sigaction() {
    if (sigaction(__signo, &__saved_action, 0) != 0)
        throw EXCEPTIONS::SystemError(errno);
}

void
Sigaction::reset() const {
    // We don't save the old action, since we already have that done
    // in the ctor.
   if (sigaction(__signo, &__current_action, 0) != 0)
        throw EXCEPTIONS::SystemError(errno);
}
