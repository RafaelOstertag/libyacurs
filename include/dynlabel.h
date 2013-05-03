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

#ifndef DYNLABEL_H
#define DYNLABEL_H

#include <string>

#include "label.h"

namespace YACURS {
    /**
     * Display text on the screen.
     *
     * Simple widget for displaying text on the screen.
     *
     * As opposed to Label, DynLabel is dynamic, and dynamically adjust to
     * available space by truncating label text.
     */
    class DynLabel : public Label {
        private:
            // Not supported
            DynLabel& operator=(const DynLabel&);

        public:
            DynLabel(const std::string& _l=std::string() );
            virtual ~DynLabel();

            // From Label
            void label(const std::string& _l);

            const std::string& label() const;

            // From WidgetBase

            void size_available(const Size& _s);

            Size size_hint() const;

            void reset_size();

            // From Realizeable

            /**
             * Refresh the label.
             *
             * Adds the Label text to the subwin.
             *
             * @param immediate not directly used by Label::refresh() but
             * passed to Widget::refresh().
             */
            void refresh(bool immediate);
    };
}

#endif // DYNLABEL_H
