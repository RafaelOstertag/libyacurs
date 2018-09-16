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

#include "gettext.h"

#include <cassert>
#include <cstdlib>
#include <stdexcept>

#include "eventqueue.h"
#include "lockscreen.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//
void LockScreen::key_event_handler(Event& e) {
    assert(e == EVT_KEY);

    if (!_unlock_dialog->shown() && _msgbox == 0) {
        _unlock_dialog->clear();
        _unlock_dialog->show();
        // Set the timeout for the unlock dialog
        EventQueue::timeout(_unlock_diag_timeout);

        // The event should not be processed any further, else the key
        // might be passed on to the Unlock Dialog. Further key
        // presses must come thru, tough.
        e.stop(true);
    }
}

void LockScreen::window_close_event_handler(Event& e) {
    assert(e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& evt = dynamic_cast<EventEx<WindowBase*>&>(e);

    if (evt.data() == _unlock_dialog) {
        if (_unlock_dialog->unlock()) {
            close();
        } else {
            if (_unlock_dialog->dialog_state() == DIALOG_OK) {
                assert(_msgbox == 0);
                _msgbox = new MessageBox(_("Unlock Failed"),
                                         _("Wrong Password"), OK_ONLY);
                _msgbox->show();
            }
        }
        // Set the timeout back to the timeout until lock screen kicks
        // in
        EventQueue::timeout(_timeout);
        return;
    }

    if (evt.data() == _msgbox) {
        delete _msgbox;
        _msgbox = 0;
    }
}

//
// Public
//

LockScreen::LockScreen(UnlockDialog* unlock, unsigned int timeout,
                       unsigned int ulck_timeout)
    : Window(Margin::zero()),
      _timeout(timeout),
      _unlock_diag_timeout(ulck_timeout),
      _unlock_dialog(unlock),
      _msgbox(0) {
    if (!_unlock_dialog)
        throw std::invalid_argument(_("InputDialog may not be 0"));
}

LockScreen::~LockScreen() {}

unsigned int LockScreen::timeout() const { return _timeout; }

unsigned int LockScreen::unlock_dialog_timeout() const {
    return _unlock_diag_timeout;
}

void LockScreen::close_unlock_dialog() {
    if (_unlock_dialog->shown())
        // We clear here, since the user might have entered the
        // password but not yet closed the dialog. If we simply would
        // close without reset, the Window Close handler would kick
        // in, ask the dialog if the password is OK (which we assume
        // it is) and unlock the screen.
        _unlock_dialog->clear();
    _unlock_dialog->close();
    if (_msgbox != 0 && _msgbox->shown()) _msgbox->close();
}

void LockScreen::show() {
    Window::show();

    // HotKey implementation does connect this event.
    // EventQueue::connect_event(EventConnectorMethod1<LockScreen>(EVT_KEY,
    // this, &LockScreen::event_key_handler));
    EventQueue::connect_event(EventConnectorMethod1<LockScreen>(
        EVT_WINDOW_CLOSE, this, &LockScreen::window_close_event_handler));

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(
        EVT_FORCEREFRESH, this, &WindowBase::force_refresh_handler));

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(
        EVT_REDRAW, this, &WindowBase::redraw_handler));

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(
        EVT_REFRESH, this, &WindowBase::refresh_handler));
    // We don't suspend this handler, since the resize should be
    // handled by windows, but not displayed. Else we would have to
    // keep track of resizes and call resize handlers upon close() of
    // LockScreen.
    //
    // EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this,&WindowBase::resize_handler));
}

void LockScreen::close() {
    // HotKey implementation disconnects this event.
    // EventQueue::disconnect_event(EventConnectorMethod1<LockScreen>(EVT_KEY,
    // this, &LockScreen::event_key_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<LockScreen>(
        EVT_WINDOW_CLOSE, this, &LockScreen::window_close_event_handler));
    EventQueue::unsuspend_all(EVT_FORCEREFRESH);
    EventQueue::unsuspend_all(EVT_REDRAW);
    EventQueue::unsuspend_all(EVT_REFRESH);

    Window::close();
}
