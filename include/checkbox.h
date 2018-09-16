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

#ifndef CHECKBOX_H
#define CHECKBOX_H 1

#include <string>
#include <vector>

#include "widget.h"

namespace YACURS {
namespace INTERNAL {
/**
 * Internal structure for CheckBox. Represents items to
 * select.
 */
struct Selectable {
    bool selected;
    std::string item;

    /**
     * Used when initializing vector.
     */
    Selectable() : selected(false) {}

    Selectable(const std::string& _str) : selected(false), item(_str) {}

    Selectable(const Selectable& _s) : selected(_s.selected), item(_s.item) {}

    Selectable& operator=(const Selectable& _s) {
        selected = _s.selected;
        item = _s.item;
        return *this;
    }
};
}  // namespace INTERNAL

/**
 */
class CheckBox : public Widget {
   private:
    /**
     * The size of the CheckBox
     */
    Size __size;

    /**
     *
     */
    unsigned short __cursor;

    std::string __title;

    CheckBox& operator=(const CheckBox& _i);

   protected:
    std::string __indicators[2];

    std::vector<INTERNAL::Selectable> __items;

    virtual void key_handler(Event& _e);

   public:
    CheckBox(const std::string& _title, const std::vector<std::string>& _items);

    virtual ~CheckBox();

    bool selected(unsigned short _i);

    bool selected(const std::string& _i);

    virtual void set_selection(unsigned short _cursor);

    virtual void set_selection(const std::string& _i);

    // From WidgetBase
    void size_available(const Size& _s);

    /**
     */
    Size size() const;

    Size size_hint() const;

    /**
     * Dummy. Does nothing.
     *
     * CheckBox is not a container Widget, hence it may not
     * notified of size changes().
     *
     * @return always @false
     */
    bool size_change();

    /**
     * Reset size.
     *
     * Does nothing.
     */
    void reset_size();

    // From Realizeable

    /**
     * Refresh the Input.
     *
     * @param immediate not directly used by Label::refresh() but
     * passed to Widget::refresh().
     */
    void refresh(bool immediate);

    void realize();

    void unrealize();
};
}  // namespace YACURS

#endif  // CHECKBOX_H
