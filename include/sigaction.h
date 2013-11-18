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
// $Id: area.h 5021 2013-04-12 07:17:47Z rafisol $

#ifndef SIGACTION_H
#define SIGACTION_H 1

#include <signal.h>

#include "yacurstypes.h"

namespace YACURS {
    namespace INTERNAL {
        /**
         * Convenience class for sigaction().
         *
         * Upon creation, it installs the new signal handler and saves
         * the previous sigaction struct.
         *
         * Upon destruction, it restores the sigaction struct using
         * the saved sigaction struct.
         */
        class Sigaction {
            private:
                /**
                 * Saved sigaction.
                 *
                 * Third argument to sigaction().
                 */
                struct sigaction __saved_action;
		
		struct sigaction __current_action;

                /**
                 * Signal Number.
                 */
                int __signo;

                Sigaction(const Sigaction&);
                Sigaction& operator=(const Sigaction&);

            public:
                /**
                 * Constructor.
                 *
                 * @param signo signal
                 *
                 * @param hndlr pointer to function handling signal.
                 *
                 * @param mask signal mask to block during execution
                 * of hndlr.
                 */
                Sigaction(int signo, sig_handler hndlr, sigset_t& mask);

                /**
                 * Restores sigaction.
                 *
                 * Restores signal action to the saved state upon
                 * invocation.
                 */
                ~Sigaction();

                /**
                 * Get the signal.
                 *
                 * @returns the signal.
                 */
                int signo() const {
                    return __signo;
                }

		/**
		 * Set the signal again
		 *
		 * Calls sigaction with the parameters received upon
		 * initialization.
		 */
		void reset() const;
        };
    } // namespace INTERNAL
} // namespace YACURS

#endif // SIGACTION_H
