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
// $Id: yacurstypes.h 5021 2013-04-12 07:17:47Z rafisol $

#ifndef YACURSCONST_H
#define YACURSCONST_H

namespace YACURS {
    /**
     * Resizing stops when any of those are underrun.
     */
    enum {
        /**
         * Minimum columns. When actual screen size underruns MIN_COLS,
         * no resize is performed.
         */
        MIN_COLS = 15,

        /**
         * Minimum rows. When actual screen size underruns MIN_ROWS,
         * no resize is performed.
         */
        MIN_ROWS = 15
    };

    /**
     * WindowBase will not realize when any those are underrun.
     */
    enum {
        /**
         * Minimum columns. When WindowBase size underruns
         * MIN_WINDOW_COLS, WindowBase will not realize.
         */
        MIN_WINDOW_COLS = 4,

        /**
         * Minimum rows. When WindowBase size underruns
         * MIN_WINDOW_ROWS, WindowBase will not realize.
         */
        MIN_WINDOW_ROWS = 1
    };

    enum {
        /**
         * Buffer size used for getcwd().
         */
        CWDBUFSZ = 1024
    };
}

#endif // YACURSCONST_H
