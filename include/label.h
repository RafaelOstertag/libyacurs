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

#ifndef LABEL_H
#define LABEL_H 1

#include <string>

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
class Label : public Widget {
   private:
    COLOROBJ _color;

    /**
     * Length of label.
     *
     * @returns length of label, if compiled with YACURS_USE_WCHAR
     * defined, takes MB characters into account.
     */
    size_t label_length() const;

   protected:
    /**
     * The text to be displayed.
     */
    std::string _label;

    /**
     * The size the Label requires. Rows are always 1. Cols are
     * _label.length(). Derrived classes may define other
     * constraints.
     */
    Size _size;

   public:
    /**
     * Constructor.
     *
     * @param l label
     */
    Label(const std::string& l = std::string());
    Label& operator=(const Label&) = delete;
    Label& operator=(Label&&) = delete;
    Label(const Label&) = delete;
    Label(Label&&) = delete;

    virtual ~Label();

    virtual void label(const std::string& l);

    virtual const std::string& label() const;

    void color(COLOROBJ c);

    COLOROBJ color() const;

    // From WidgetBase

    /**
     * Size the Label requires.
     *
     * @return Size::rows() always 1. Size::cols() equal the
     * length of the string to be displayed.
     */
    Size size() const;

    Size size_hint() const;

    /**
     * Dummy. Does nothing.
     *
     * Label is not a container Widget, hence it may not notified
     * of size changes().
     *
     * @return always @false
     */
    bool size_change();

    /**
     * Does nothing.
     *
     * Does nothing on a Label, but we have to override.
     */
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
}  // namespace YACURS

#endif  // LABEL_H
