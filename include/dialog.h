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

#ifndef DIALOG_H
#define DIALOG_H 1

#include <string>

#include "yacurstypes.h"
#include "window.h"
#include "hrule.h"
#include "vpack.h"
#include "hpack.h"
#include "button.h"

namespace YACURS {
    namespace INTERNAL {
        class HotKeyEsc;
    }

    class Dialog : public Window {
        private:
            VPack* __vpack;
            HPack* __hpack;
	    Button* __byes;
            Button* __bok;
            Button* __bcancel;
	    Button* __bno;
	    HRule* __hrule;
            DIALOG_STATE __dstate;
            DIALOG_TYPE __dialog_type;
            DIALOG_SIZE __dialog_size;

            std::string __title;

            // Not supported
            Dialog(const Dialog&);
            Dialog& operator=(const Dialog&);

        protected:
            const Button* const ok_button() const;

            const Button* const cancel_button() const;

            const Button* const yes_button() const;

            const Button* const no_button() const;

            void dialog_state(DIALOG_STATE st);

	    /**
	     * Add a button to the button pack.
	     *
	     * @param b button to be added.
	     */
	    void add_button(Button* b);

            virtual void button_press_handler(Event& _e);

	    /**
	     * Called on OK button press.
	     *
	     * Will be called on OK button press.
	     * To be implemented by user if needed.
	     */
	    virtual void on_ok_button();

	    /**
	     * Called on Yes button press.
	     *
	     * Will be called on Yes button press.
	     * To be implemented by user if needed.
	     */
	    virtual void on_yes_button();

	    /**
	     * Called on Cancel button press.
	     *
	     * Will be called on Cancel button press.
	     * To be implemented by user if needed.
	     */
	    virtual void on_cancel_button();

	    /**
	     * Called on No button press.
	     *
	     * Will be called on No button press.
	     * To be implemented by user if needed.
	     */
	    virtual void on_no_button();

        public:
            Dialog(const std::string& _title,
                   DIALOG_TYPE _dt=OKCANCEL,
                   DIALOG_SIZE _ds=AUTOMATIC);
            virtual ~Dialog();
            DIALOG_STATE dialog_state() const;
	    DIALOG_TYPE dialog_type() const;

	    void title(const std::string& _title);

	    std::string title() const;
	    

	    /**
	     * Add widget
	     *
	     * Add widget to front of vpack.
	     *
	     * @param _w widget to be added.
	     */
            void widget(WidgetBase* _w);

            // Those are from Realizable
            void refresh(bool immediate);

            // Does nothing, everything handled in parent.
            //void resize(const Area& _a);
            void realize();

            void unrealize();

            friend class INTERNAL::HotKeyEsc;
    };
}

#endif // DIALOG_H
