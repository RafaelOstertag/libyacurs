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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gettext.h>

#include <cassert>
#include <cstdlib>

#include "dialog.h"
#include "eventqueue.h"
#include "yacursex.h"
#include "hotkey.h"

namespace YACURS {
    namespace INTERNAL {
        /**
         * HotKey for cancelling Dialog.
         */
        class HotKeyEsc : public HotKey {
            private:
                Dialog& __dialog;

            public:
                HotKeyEsc(Dialog& d) : HotKey(27),
                    __dialog(d) {
                }

                HotKeyEsc(const HotKeyEsc& hk) : HotKey(hk),
                    __dialog(hk.__dialog) {
                }

                void action() {
                    __dialog.__dstate = DIALOG_CANCEL;
                    __dialog.close();
                }

                HotKey* clone() const {
                    return new HotKeyEsc(*this);
                }
        };
    } // namespace INTERNAL
} // namespace YACURS

using namespace YACURS;

//
// Private
//
Dialog::Dialog(const Dialog&) {
    throw EXCEPTIONS::NotSupported();
}

Dialog&
Dialog::operator=(const Dialog&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
const Button* const
Dialog::ok_button() const {
    return __bok;
}

const Button* const
Dialog::cancel_button() const {
    return __bcancel;
}

const Button* const
Dialog::yes_button() const {
    return __byes;
}

const Button* const
Dialog::no_button() const {
    return __bno;
}

void
Dialog::dialog_state(DIALOG_STATE st) {
    __dstate = st;
}

void
Dialog::add_button(Button* b) {
    __hpack->add_back(b);
}

void
Dialog::add_button(Spacer* s) {
    __hpack->add_back(s);
}

void
Dialog::button_press_handler(Event& _e) {
    if (realization() != REALIZED) return;

    assert(_e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(_e);

    if (evt.data() == __bok) {
        __dstate = DIALOG_OK;
	on_ok_button();
	close();
    }

    if (evt.data() == __bcancel) {
        __dstate = DIALOG_CANCEL;
	on_cancel_button();
	close();
    }

    if (evt.data() == __byes) {
        __dstate = DIALOG_YES;
	on_yes_button();
	close();
    }

    if (evt.data() == __bno) {
        __dstate = DIALOG_NO;
	on_no_button();
	close();
    }
}

void
Dialog::on_ok_button() {
    /* Intentionally empty */
}

void
Dialog::on_cancel_button() {
    /* Intentionally empty */
}

void
Dialog::on_yes_button() {
    /* Intentionally empty */
}

void
Dialog::on_no_button() {
    /* Intentionally empty */
}

//
// Public
//

Dialog::Dialog(const std::string& _title,
               DIALOG_TYPE _dt,
               DIALOG_SIZE _ds) : __vpack(new VPack),
				  __hpack(new HPack),
				  __byes(0),
				  __bok(0),
				  __ok_spacer(new Spacer),
				  __bcancel(0),
				  __bno(0),
				  __hrule(new HRule),
				  __dstate(DIALOG_CANCEL),
				  __dialog_type(_dt),
				  __dialog_size(_ds),
				  __title(_title) {
    __hrule->color(DIALOG);

    __vpack->add_back(__hrule);
    __vpack->add_back(__hpack);

    switch (__dialog_type) {
    case OKCANCEL:
        __bcancel = new Button(_("Cancel") );
        __hpack->add_back(__bcancel);
	__hpack->add_front(__ok_spacer);
	// Fall thru
	
    case OK_ONLY:
        __bok = new Button(_("OK") );
        __hpack->add_front(__bok);
        break;
	
    case YESNO:
        __bno = new Button(_("No") );
        __hpack->add_front(__bno);
	__hpack->add_front(__ok_spacer);
	// Fall thru
	
    case YES_ONLY:
        __byes = new Button(_("Yes") );
        __hpack->add_front(__byes);
        break;
	
	// handled separately
    case YESNOCANCEL:
        __bcancel = new Button(_("Cancel") );
        __hpack->add_back(__bcancel);
	__hpack->add_front(__ok_spacer);
        __bno = new Button(_("No") );
        __hpack->add_front(__bno);
	__hpack->add_front(__ok_spacer);
        __byes = new Button(_("Yes") );
        __hpack->add_front(__byes);
	break;

    default:
        throw EXCEPTIONS::InvalidDialogType();
        break;
    }

    // from Window
    Window::widget(__vpack);
    frame(true);
    color(DIALOG);
}

Dialog::~Dialog() {
    assert(__vpack != 0);
    assert(__hpack != 0);
    assert(__hrule != 0);
    assert(__ok_spacer != 0);

    delete __vpack;
    delete __hpack;
    delete __hrule;
    delete __ok_spacer;

    if (__bok)
	delete __bok;
    if (__byes)
	delete __byes;
    if (__bcancel)
	delete __bcancel;
    if (__bno)
	delete __bno;

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(
                                     EVT_BUTTON_PRESS, this,
                                     &Dialog::button_press_handler) );
}

void
Dialog::widget(WidgetBase* _w) {
    assert(__vpack != 0);
    __vpack->add_front(_w);
}

DIALOG_STATE
Dialog::dialog_state() const {
    return __dstate;

}

DIALOG_TYPE
Dialog::dialog_type() const {
    return __dialog_type;
}

void
Dialog::title(const std::string& _title) {
    __title = _title;

    if (realization() == REALIZED) {
	// Ugly hack, reset box, so that it will be adjusted to new
	// title...
	if (frame())
	    curses_window()->box();

	refresh(true);
    }
}

std::string
Dialog::title() const {
    return __title;
}

void
Dialog::refresh(bool immediate) {
    if (realization() != REALIZED &&
        realization() != REALIZING) return;

    CurStr tmp(__title, Coordinates(1, 0), DIALOG_TITLE);
    curses_window()->addstrx(tmp);

    Window::refresh(immediate);
}

void
Dialog::realize() {
    REALIZE_ENTER;

    // Reset the dialog state
    __dstate = DIALOG_CANCEL;

    EventQueue::connect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS,
                                                            this,
                                                            &Dialog::
                                                            button_press_handler) );

    Margin _margin(2, 2, 2, 2);
    __vpack->hinting(false);

    if (__dialog_size == AUTOMATIC) {
        __vpack->hinting(true);
        // Compute the margin. We try to vertically center the dialog.
        int hinted_rows = Window::widget()->size_hint().rows();
        if (hinted_rows > 0 && hinted_rows < area().rows() - 2) {
            int vert_margin = (area().rows() - hinted_rows) / 2 - 1;
            _margin = Margin(vert_margin, 2, vert_margin, 2);
        }
    }

    margin(_margin);
    Window::realize();

    add_hotkey(INTERNAL::HotKeyEsc(*this) );

    REALIZE_LEAVE;
}

void
Dialog::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(
                                     EVT_BUTTON_PRESS, this,
                                     &Dialog::button_press_handler) );

    Window::unrealize();

    UNREALIZE_LEAVE;
}
