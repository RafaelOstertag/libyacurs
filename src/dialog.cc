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
#include "hotkey.h"
#include "yacursex.h"

namespace YACURS {
namespace INTERNAL {
/**
 * HotKey for cancelling Dialog.
 */
class HotKeyEsc : public HotKey {
   private:
    Dialog& _dialog;

   public:
    HotKeyEsc(Dialog& d) : HotKey(27), _dialog(d) {}

    HotKeyEsc(const HotKeyEsc& hk) : HotKey(hk), _dialog(hk._dialog) {}

    void action() {
        _dialog._dstate = DIALOG_CANCEL;
        _dialog.close();
    }

    HotKey* clone() const { return new HotKeyEsc(*this); }
};
}  // namespace INTERNAL
}  // namespace YACURS

using namespace YACURS;

//
// Private
//

//
// Protected
//
const Button* const Dialog::ok_button() const { return _bok; }

const Button* const Dialog::cancel_button() const { return _bcancel; }

const Button* const Dialog::yes_button() const { return _byes; }

const Button* const Dialog::no_button() const { return _bno; }

void Dialog::dialog_state(DIALOG_STATE st) { _dstate = st; }

void Dialog::add_button(Button* b) { _hpack->add_back(b); }

void Dialog::add_button(Spacer* s) { _hpack->add_back(s); }

void Dialog::button_press_handler(Event& e) {
    if (realization() != REALIZED) return;

    assert(e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(e);

    if (evt.data() == _bok) {
        _dstate = DIALOG_OK;
        on_ok_button();
        close();
    }

    if (evt.data() == _bcancel) {
        _dstate = DIALOG_CANCEL;
        on_cancel_button();
        close();
    }

    if (evt.data() == _byes) {
        _dstate = DIALOG_YES;
        on_yes_button();
        close();
    }

    if (evt.data() == _bno) {
        _dstate = DIALOG_NO;
        on_no_button();
        close();
    }
}

void Dialog::on_ok_button() { /* Intentionally empty */
}

void Dialog::on_cancel_button() { /* Intentionally empty */
}

void Dialog::on_yes_button() { /* Intentionally empty */
}

void Dialog::on_no_button() { /* Intentionally empty */
}

//
// Public
//

Dialog::Dialog(const std::string& title, DIALOG_TYPE dialogType,
               DIALOG_SIZE dialogSize)
    : _vpack(new VPack),
      _hpack(new HPack),
      _byes(0),
      _bok(0),
      _ok_spacer(new Spacer),
      _bcancel(0),
      _bno(0),
      _hrule(new HRule),
      _dstate(DIALOG_CANCEL),
      _dialog_type(dialogType),
      _dialog_size(dialogSize),
      _title(title) {
    _hrule->color(DIALOG);

    _vpack->add_back(_hrule);
    _vpack->add_back(_hpack);

    switch (_dialog_type) {
        case OKCANCEL:
            _bcancel = new Button(_("Cancel"));
            _hpack->add_back(_bcancel);
            _hpack->add_front(_ok_spacer);
            // Fall thru

        case OK_ONLY:
            _bok = new Button(_("OK"));
            _hpack->add_front(_bok);
            break;

        case YESNO:
            _bno = new Button(_("No"));
            _hpack->add_front(_bno);
            _hpack->add_front(_ok_spacer);
            // Fall thru

        case YES_ONLY:
            _byes = new Button(_("Yes"));
            _hpack->add_front(_byes);
            break;

        // handled separately
        case YESNOCANCEL:
            _bcancel = new Button(_("Cancel"));
            _hpack->add_back(_bcancel);
            _hpack->add_front(_ok_spacer);
            _bno = new Button(_("No"));
            _hpack->add_front(_bno);
            _hpack->add_front(_ok_spacer);
            _byes = new Button(_("Yes"));
            _hpack->add_front(_byes);
            break;

        default:
            throw EXCEPTIONS::InvalidDialogType();
            break;
    }

    // from Window
    Window::widget(_vpack);
    frame(true);
    color(DIALOG);
}

Dialog::~Dialog() {
    assert(_vpack != 0);
    assert(_hpack != 0);
    assert(_hrule != 0);
    assert(_ok_spacer != 0);

    delete _vpack;
    delete _hpack;
    delete _hrule;
    delete _ok_spacer;

    if (_bok) delete _bok;
    if (_byes) delete _byes;
    if (_bcancel) delete _bcancel;
    if (_bno) delete _bno;

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(
        EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));
}

void Dialog::widget(WidgetBase* _w) {
    assert(_vpack != 0);
    _vpack->add_front(_w);
}

DIALOG_STATE
Dialog::dialog_state() const { return _dstate; }

DIALOG_TYPE
Dialog::dialog_type() const { return _dialog_type; }

void Dialog::title(const std::string& title) {
    _title = title;

    if (realization() == REALIZED) {
        // Ugly hack, reset box, so that it will be adjusted to new
        // title...
        if (frame()) curses_window()->box();

        refresh(true);
    }
}

std::string Dialog::title() const { return _title; }

void Dialog::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    CurStr tmp(_title, Coordinates(1, 0), DIALOG_TITLE);
    curses_window()->addstrx(tmp);

    Window::refresh(immediate);
}

void Dialog::realize() {
    REALIZE_ENTER;

    // Reset the dialog state
    _dstate = DIALOG_CANCEL;

    EventQueue::connect_event(EventConnectorMethod1<Dialog>(
        EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));

    Margin _margin(2, 2, 2, 2);
    _vpack->hinting(false);

    if (_dialog_size == AUTOMATIC) {
        _vpack->hinting(true);
        // Compute the margin. We try to vertically center the dialog.
        int hinted_rows = Window::widget()->size_hint().rows();
        if (hinted_rows > 0 && hinted_rows < area().rows() - 2) {
            int vert_margin = (area().rows() - hinted_rows) / 2 - 1;
            _margin = Margin(vert_margin, 2, vert_margin, 2);
        }
    }

    margin(_margin);
    Window::realize();

    add_hotkey(INTERNAL::HotKeyEsc(*this));

    REALIZE_LEAVE;
}

void Dialog::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(
        EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));

    Window::unrealize();

    UNREALIZE_LEAVE;
}
