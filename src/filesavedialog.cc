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
#include <cerrno>
#include <cstdlib>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <unistd.h>

#include "filesavedialog.h"
#include "yacursconst.h"

using namespace YACURS;

//
// Private
//

void FileSaveDialog::window_close_handler(Event& e) {
    FileDialog::window_close_handler(e);

    assert(e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(e);

    if (_errmsgbox == _evt.data()) {
        delete _errmsgbox;
        _errmsgbox = 0;
        return;
    }

    if (_confirmdia != 0 && _confirmdia == _evt.data()) {
        if (_confirmdia->dialog_state() == DIALOG_YES) {
            // It is ok to overwrite
            dialog_state(DIALOG_OK);
            close();
        }
        delete _confirmdia;
        _confirmdia = 0;
        return;
    }
}

//
// Protected
//
void FileSaveDialog::button_press_handler(Event& e) {
    // We hook in the button_press_handler of Dialog, so we can test
    // if the file already exists when the ok button is pressed.

    if (realization() != REALIZED) return;

    assert(e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(e);

    if (evt.data() == ok_button() || evt.data() == yes_button()) {
        // This is also called in
        // FileDialog::button_press_handler(). But since
        // FileDialog::button_press_handler() closes the dialog if the
        // selection type matches (we don't want that, we also have to
        // check for existing file), we call it ourselves and avoid
        // FileDialog::button_press_handler() till the end of the
        // method.
        if (!selection_type_match()) {
            // the selection did not match the requested selection
            // type. The method has raised a dialog to indicate the
            // missmatch to the user. So we have to return and do
            // nothing.
            return;
        }

        struct stat wdc;
        int retval;
        if ((retval = stat(filepath().c_str(), &wdc)) == 0) {
            assert(_confirmdia == 0);
            _confirmdia = new MessageBox2(
                _("Confirmation"), filepath(),
                _("already exists. Do you want to overwrite?"), YESNO);
            _confirmdia->show();
            return;
        } else {
            switch (errno) {
                case EACCES:
                    _errmsgbox = new MessageBox2(
                        _("Error"), filepath(),
                        EXCEPTIONS::SystemError(errno).what(), OK_ONLY);
                    _errmsgbox->show();
                    return;
                    break;

                case ENOENT:
                    // that's fine, fall through to
                    // Dialog::button_press_handler
                    break;

                default:
                    throw EXCEPTIONS::SystemError(errno);
                    break;
            }
        }
    }

    FileDialog::button_press_handler(e);
}

//
// Public
//

FileSaveDialog::FileSaveDialog(std::string path, bool do_chdir, DIALOG_TYPE dt)
    : FileDialog(std::string(_("Save File")), path, do_chdir, dt),
      _confirmdia(0),
      _errmsgbox(0) {
    // Only allow files
    selection_type(YACURS::FILE);

    EventQueue::connect_event(EventConnectorMethod1<FileSaveDialog>(
        EVT_WINDOW_CLOSE, this, &FileSaveDialog::window_close_handler));
}

FileSaveDialog::~FileSaveDialog() {
    EventQueue::disconnect_event(EventConnectorMethod1<FileSaveDialog>(
        EVT_WINDOW_CLOSE, this, &FileSaveDialog::window_close_handler));

    if (_confirmdia) delete _confirmdia;
    if (_errmsgbox) delete _errmsgbox;
}
