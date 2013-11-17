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

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
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

std::string
FileSaveDialog::dir_up(const std::string& dir) {
    std::string::size_type pos = dir.rfind('/');

    std::string retval;

    if (pos != 0) {
        retval = dir.substr(0, pos);
    } else {
        // pos==0 is the root directory, which we want
        // to preserve
        retval = "/";
    }

    return retval;
}

void
FileSaveDialog::read_dir() {
    std::list<std::string> _dirs;
    std::list<std::string> _files;

    if (__path->label().empty() ) {
        char buff[CWDBUFSZ];
        if (!getcwd(buff, CWDBUFSZ) ) {
            if (errno == EACCES)
                __path->label("/");
            else
                throw EXCEPTIONS::SystemError(errno);
        }
        __path->label(buff);
    }

    DIR* dir = opendir(__path->label().c_str() );
    if (dir == 0)
        throw EXCEPTIONS::SystemError(errno);

    std::string _base(__path->label() );
    assert(_base.length() > 0);

    // If _base.length()==1, then it is the root directory and we
    // don't append a slash
    if (_base.length() > 1)
        _base += "/";

    dirent* dent;
    while (errno = 0, dent = readdir(dir) ) {
        std::string _tmp(_base + dent->d_name);

        struct stat _stat;
        if (stat(_tmp.c_str(), &_stat) == -1) {
            // reset errno
            errno = 0;
            continue;
        }

        if (_stat.st_mode & S_IFDIR) {
            _dirs.push_back(dent->d_name);
            continue;
        }
        if (_stat.st_mode & S_IFREG) {
            _files.push_back(dent->d_name);
            continue;
        }
    }

    // On Fedora 18, somewhere in __directories->set(_dirs), errno
    // will be updated, so we save it here, before proceeding
    int errno_save = errno;

    __directories->set(_dirs);
    __files->set(_files);

    if (errno_save != 0) {
        (void)closedir(dir);
        throw EXCEPTIONS::SystemError(errno);
    }

    if (closedir(dir) != 0)
        throw EXCEPTIONS::SystemError(errno);
}

void
FileSaveDialog::listbox_enter_handler(Event& _e) {
    assert(_e == EVT_LISTBOX_ENTER);

    EventEx<ListBox<>*>& _evt = dynamic_cast<EventEx<ListBox<>*>&>(_e);

    if (__directories == _evt.data() ) {
        if (__directories->selected() == ".")
            return;

        if (__directories->selected() == "..") {
            __path->label(dir_up(__path->label() ) );
        } else {
            // If __path->label().length()==1, then it is the root
            // directory and we don't append a slash
            if (__path->label().length() > 1)
                __path->label(__path->label() + "/" +
                              __directories->selected() );
            else
                __path->label(__path->label() +
                              __directories->selected() );
        }
        try {
            read_dir();
        } catch (EXCEPTIONS::SystemError& ex) {
            std::string _tmp(_("Cannot change to ") + __path->label() + ":");
            __errmsgbox = new MessageBox2(_("System Error"),
                                          _tmp,
                                          ex.what(),
                                          OK_ONLY);
            __errmsgbox->show();

            // Most likely the directory is not accessible, so go
            // one up. If we happen to reach the root directory,
            // and that's not accessible too, the system probably
            // is messed up...
            __path->label(dir_up(__path->label() ) );
        }
        return;
    }

    if (__files == _evt.data() ) {
        __filename->input(__files->selected() );
    }
}

void
FileSaveDialog::window_close_handler(Event& _e) {
    assert(_e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(_e);

    if (__errmsgbox != 0 && __errmsgbox == _evt.data() ) {
        delete __errmsgbox;
        __errmsgbox = 0;
        return;
    }

    if (__confirmdia != 0 && __confirmdia == _evt.data() ) {
        if (__confirmdia->dialog_state() == DIALOG_OK) {
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

    assert(_e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(_e);

    // Take into account that dialog type can be specified by user
    if (evt.data() == ok_button() ||
	evt.data() == yes_button() ) {
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

    Dialog::button_press_handler(_e);
}

//
// Public
//

FileSaveDialog::FileSaveDialog(std::string _path,
                               DIALOG_TYPE _dt) :
    Dialog(std::string(_("Save File") ), _dt,
           Dialog::FULLSIZE),
    __errmsgbox(0),
    __confirmdia(0),
    __path(0),
    __directories(0),
    __files(0),
    __filename(0),
    __hpack(0),
    __vpack(0) {
    __vpack = new VPack;
    __hpack = new HPack;

    __path = new DynLabel(_path);
    __path->color(DIALOG);

    __directories = new ListBox<>;
    __directories->sort_order(ASCENDING);

    __files = new ListBox<>;
    __files->sort_order(ASCENDING);

    __hpack->add_back(__directories);
    __hpack->add_back(__files);

    __hpack->always_dynamic(true);
    __hpack->hinting(false);

    __vpack->add_back(__path);
    __vpack->add_back(__hpack);

    __filename = new Input<>;

    __vpack->add_back(__filename);

    __vpack->always_dynamic(true);
    __vpack->hinting(false);

    widget(__vpack);

    EventQueue::connect_event(EventConnectorMethod1<FileSaveDialog>(
                                  EVT_LISTBOX_ENTER, this,
                                  &FileSaveDialog::listbox_enter_handler) );
    EventQueue::connect_event(EventConnectorMethod1<FileSaveDialog>(
                                  EVT_WINDOW_CLOSE, this,
                                  &FileSaveDialog::window_close_handler) );
}

FileSaveDialog::~FileSaveDialog() {
    EventQueue::disconnect_event(EventConnectorMethod1<FileSaveDialog>(
                                     EVT_LISTBOX_ENTER, this,
                                     &FileSaveDialog::listbox_enter_handler) );
    EventQueue::disconnect_event(EventConnectorMethod1<FileSaveDialog>(
                                     EVT_WINDOW_CLOSE, this,
                                     &FileSaveDialog::window_close_handler) );

    assert(__path != 0);
    assert(__directories != 0);
    assert(__files != 0);
    assert(__filename != 0);
    assert(__hpack != 0);
    assert(__vpack != 0);

    delete __path;
    delete __directories;
    delete __files;
    delete __filename;
    delete __hpack;
    delete __vpack;
}

std::string
FileSaveDialog::filepath() const {
    assert(__path != 0);
    assert(__directories != 0);
    assert(__files != 0);
    assert(__filename != 0);

    std::string retval;

    if (__path->label() == "/")
        retval = __path->label() + __filename->input();
    else
        retval = __path->label() + "/" + __filename->input();

    return retval;
}

const std::string&
FileSaveDialog::directory() const {
    assert(__path != 0);
    assert(__directories != 0);
    assert(__files != 0);
    assert(__filename != 0);

    return __path->label();
}

const std::string&
FileSaveDialog::filename() const {
    return __filename->input();
}

void
FileSaveDialog::refresh(bool immediate) {
    Dialog::refresh(immediate);

    try {
        read_dir();
    } catch (EXCEPTIONS::SystemError& ex) {
        std::string _tmp(_("Cannot change to ") + __path->label() + ":");
        __errmsgbox = new MessageBox2(_("System Error"),
                                      _tmp,
                                      ex.what(),
                                      OK_ONLY);
        __errmsgbox->show();

        // Most likely the directory is not accessible, so go one
        // up. If we happen to reach the root directory, and
        // that's not accessible too, the system probably is
        // messed up...
        __path->label(dir_up(__path->label() ) );
    }
}
