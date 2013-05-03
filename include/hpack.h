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

#ifndef HPACK_H
#define HPACK_H

#include "pack.h"

namespace YACURS {
    /**
     * Packs Widgets horizontally.
     *
     * The main purpose of this class is the implement the size
     * calculation of and for the associated Widgets and to implement
     * realize().
     */
    class HPack : public Pack {
        private:
            // Not supported
            HPack& operator=(const HPack&);

        protected:
            void recalc_size();

            Size calc_size_non_dynamic() const;

        public:
            virtual ~HPack();

            Size size_hint() const;

            void realize();
    };
}

#endif // HPACK_H
