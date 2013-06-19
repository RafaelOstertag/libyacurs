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

#include "window.h"
#include "vpack.h"
#include "hpack.h"
#include "button.h"

namespace YACURS {
    namespace INTERNAL {
        class HotKeyEsc;
    }

    class Dialog : public Window {
        public:
	    /**
	     * State of dialog.
	     */
            enum STATE {
		/// Dialog was confirmed
                DIALOG_OK,
		/// Dialog was cancelled
                DIALOG_CANCEL
            };

            enum DIALOG_TYPE {
                OK_ONLY,
                YES_ONLY,
                OKCANCEL,
                YESNO
            };

            enum DIALOG_SIZE {
                AUTOMATIC,
                FULLSIZE
            };

        private:
            VPack* __vpack;
            HPack* __hpack;
            Button* __bok;
            Button* __bcancel;
            STATE __dstate;
            DIALOG_TYPE __dialog_type;
            DIALOG_SIZE __dialog_size;

            std::string __title;

            // Not supported
            Dialog(const Dialog&);
            Dialog& operator=(const Dialog&);

        protected:
            const Button* const ok_button() const;

            const Button* const cancel_button() const;

            void dialog_state(STATE st);

            virtual void button_press_handler(Event& _e);

	    /**
	     * Called on close.
	     *
	     * Will be called before dialog closes.
	     * To be implemented by user if needed.
	     *
	     * @param st state. See STATE for possible values.
	     */
	    virtual void on_close(STATE st);

	    /**
	     * Called on OK button press.
	     *
	     * Will be called on OK button press.
	     * To be implemented by user if needed.
	     */
	    virtual void on_ok_button();

	    /**
	     * Called on Cancel button press.
	     *
	     * Will be called on Cancel button press.
	     * To be implemented by user if needed.
	     */
	    virtual void on_cancel_button();

        public:
            Dialog(const std::string& _title,
                   DIALOG_TYPE _dt=OKCANCEL,
                   DIALOG_SIZE _ds=AUTOMATIC);
            virtual ~Dialog();
            STATE dialog_state() const;

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
