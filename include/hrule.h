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

#ifndef HRULE_H
#define HRULE_H 1

#include "widget.h"

namespace YACURS {
    /**
     * Display text on the screen.
     *
     * Simple widget for displaying text on the screen.
     *
     * Label is not dynamic, i.e. if the text is too long, it might happen
     * that it cannot be realized() and throwing an exception.
     */
    class HRule : public Widget {
        private:
            // Not supported
            HRule& operator=(const HRule&);

            COLOROBJ __color;

        protected:
            /**
             * The size the Label requires. Rows are always 1. Cols are
             * __label.length(). Derrived classes may define other
             * constraints.
             */
            Size __size;

        public:
            /**
             * Constructor.
             *
             * @param _l label
             */
            HRule();

            virtual ~HRule();

            void color(COLOROBJ c);

            COLOROBJ color() const;

            // From WidgetBase

            Size size() const;

            Size size_hint() const;

            /**
             * Dummy. Does nothing.
             *
             * HRule is not a container Widget, hence it may not notified
             * of size changes().
             *
             * @return always @false
             */
            bool size_change();

            void reset_size();

            // From Realizeable

            /**
             * Refresh the HRule.
             *
             * Adds the HRule text to the subwin.
             *
             * @param immediate not directly used by HRule::refresh()
             * but passed to Widget::refresh().
             */
            void refresh(bool immediate);
    };
}

#endif // HRULE_H
