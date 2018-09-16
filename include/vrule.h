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

#ifndef VRULE_H
#define VRULE_H 1

#include "rule.h"

namespace YACURS {
/**
 * Display text on the screen.
 *
 * Simple widget for displaying text on the screen.
 *
 * Label is not dynamic, i.e. if the text is too long, it might happen
 * that it cannot be realized() and throwing an exception.
 */
class VRule : public Rule {
   public:
    /**
     * Constructor.
     *
     * @param _l label
     */
    VRule();
    VRule& operator=(const VRule&) = delete;
    VRule& operator=(VRule&&) = delete;
    VRule(const VRule&) = delete;
    VRule(VRule&&) = delete;

    virtual ~VRule();

    // From WidgetBase

    void size_available(const Size& s);

    Size size_hint() const;

    // From Realizeable

    /**
     * Refresh the VRule.
     *
     * Adds the VRule text to the subwin.
     *
     * @param immediate not directly used by VRule::refresh()
     * but passed to Widget::refresh().
     */
    void refresh(bool immediate);
};
}  // namespace YACURS

#endif  // VRULE_H
