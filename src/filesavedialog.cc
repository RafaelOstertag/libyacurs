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

#include <cstdlib>
#include <cassert>
#include <cerrno>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include <unistd.h>
#include <climits>
#include <dirent.h>

#include "filesavedialog.h"
#include "yacursconst.h"

using namespace YACURS;

//
// Private
//
FileSaveDialog&
FileSaveDialog::operator=(const FileSaveDialog&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

void
FileSaveDialog::window_close_handler(Event& _e) {
    FileDialog::window_close_handler(_e);

    assert(_e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(_e);

    if (__errmsgbox == _evt.data()) {
	delete __errmsgbox;
	__errmsgbox=0;
	return;
    }

    if (__confirmdia != 0 && __confirmdia == _evt.data() ) {
        if (__confirmdia->dialog_state() == DIALOG_YES) {
            // It is ok to overwrite
            dialog_state(DIALOG_OK);
            close();
        }
        delete __confirmdia;
        __confirmdia = 0;
        return;
    }
}

//
// Protected
//
void
FileSaveDialog::button_press_handler(Event& _e) {
    // We hook in the button_press_handler of Dialog, so we can test
    // if the file already exists when the ok button is pressed.

    if (realization() != REALIZED) return;

    FileDialog::button_press_handler(_e);

    assert(_e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(_e);

    // Take into account that dialog type can be specified by user.
    // Further, if the event is stopped, the FileDialog button press
    // handler already did something (e.g. displaying that selection
    // is not a file) so we don't touch it.
    if ((evt.data() == ok_button() ||
	 evt.data() == yes_button()) &&
	!_e.stop()) {
        struct stat wdc;
        int retval;
        if ( (retval = stat(filepath().c_str(), &wdc) ) == 0) {
            assert(__confirmdia == 0);
            __confirmdia = new MessageBox2(_("Confirmation"),
                                           filepath(),
                                           _(
                                               "already exists. Do you want to overwrite?"),
                                           YESNO);
            __confirmdia->show();
            _e.stop(true);
            return;
        } else {
            int s_errno = errno;
            switch (s_errno) {
            case EACCES:
                __errmsgbox = new MessageBox2(_("Error"),
                                              filepath(),
                                              EXCEPTIONS::SystemError(
                                                  s_errno).what(),
                                              OK_ONLY);
                __errmsgbox->show();
                return;
                break;

            case ENOENT:
                // that's fine, fall through to
                // Dialog::button_press_handler
                break;

            default:
                throw EXCEPTIONS::SystemError(s_errno);
                break;
            }
        }
    }
}

//
// Public
//

FileSaveDialog::FileSaveDialog(std::string _path,
			       bool _do_chdir,
                               DIALOG_TYPE _dt) :
    FileDialog(std::string(_("Save File") ), _path, _do_chdir, _dt),
    __confirmdia(0),
    __errmsgbox(0) {

    // Only allow files
    selection_type(YACURS::FILE);

    EventQueue::connect_event(EventConnectorMethod1<FileSaveDialog>(
                                  EVT_WINDOW_CLOSE, this,
                                  &FileSaveDialog::window_close_handler) );
}

FileSaveDialog::~FileSaveDialog() {
    EventQueue::disconnect_event(EventConnectorMethod1<FileSaveDialog>(
                                     EVT_WINDOW_CLOSE, this,
                                     &FileSaveDialog::window_close_handler) );

    if (__confirmdia) delete __confirmdia;
    if (__errmsgbox) delete __errmsgbox;
}
