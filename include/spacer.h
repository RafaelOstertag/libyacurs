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

#ifndef SPACER_H
#define SPACER_H 1

#include <list>

#include "area.h"
#include "widgetbase.h"

namespace YACURS {
/**
 * A 1x1 spacer
 */
class Spacer : public WidgetBase {
   private:
   public:
    Spacer();
    Spacer& operator=(const Spacer&) = delete;
    Spacer& operator=(Spacer&&) = delete;
    Spacer(const Spacer&) = delete;
    Spacer(Spacer&&) = delete;
    virtual ~Spacer();

    // from WidgetBase
    bool size_change();
    Size size() const;
    Size size_hint() const;
    void reset_hint();
    void reset_size();
    bool can_focus() const;

    // from Realizeable
    void refresh(bool);
    void resize(const Area&);
    void realize();
};
}  // namespace YACURS

#endif  // PACKSPACER_H
