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

#ifndef BUTTON_H
#define BUTTON_H 1

#include "event.h"
#include "label.h"

namespace YACURS {
/**
 * A button that can be pressed.
 *
 * Upon press, a button emits @c EVT_BUTTON_PRESS. All Event
 * Connectors will receive that event. The can distinguish between
 * different buttons by comparing EventEx<Button*>::data() to the
 * address of the Button they're interested in.
 */
class Button : public Label {
   private:
    /**
     * Flag whether or not button is enabled
     */
    bool __enabled;

    /**
     * Temporary string used when returning label.
     */
    std::string __tmp_label;

    // Not supported
    Button& operator=(const Button&);

   protected:
    /**
     * Key Handler.
     *
     * Handles @c EVT_KEY events.
     *
     * @param _e EventEx<int>.
     */
    virtual void key_handler(Event& _e);

    // From Realizeable
    virtual void realize();

    virtual void unrealize();

   public:
    /**
     * Constructor.
     *
     * @param _b label
     */
    Button(const std::string& _b);

    virtual ~Button();

    /**
     * Set label.
     *
     * @param _l label
     */
    void label(const std::string& _l);

    /**
     * Get label.
     *
     * @return label of Button.
     */
    const std::string& label() const;

    void enabled(bool _f);

    bool enabled() const;

    // From Realizeable
    void refresh(bool immediate);
};
}  // namespace YACURS

#endif  // BUTTON_H
