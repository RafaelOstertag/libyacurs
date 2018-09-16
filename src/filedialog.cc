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
// $Id: filesavedialog.cc 5702 2013-11-26 18:18:33Z rafi $

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <cassert>
#include <cerrno>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <dirent.h>
#include <unistd.h>
#include <climits>

#include "filedialog.h"
#include "yacursconst.h"

using namespace YACURS;

//
// Private
//
FileDialog& FileDialog::operator=(const FileDialog&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

std::string FileDialog::cleanup_path(const std::string& p) const {
    if (p.empty()) return p;

    std::string working_copy(p);

    if (working_copy.find("//") == std::string::npos) return working_copy;

    std::string::size_type pos = 0;

    while ((pos = working_copy.find("//")) != std::string::npos)
        working_copy = working_copy.replace(pos, 2, "/");

    return working_copy;
}

//
// Protected
//
std::string FileDialog::dir_up(const std::string& dir) {
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

void FileDialog::read_dir() {
    std::list<std::string> _dirs;
    std::list<std::string> _files;

    if (__path.label().empty()) {
        try {
            __path.label(getcwd());
        } catch (EXCEPTIONS::SystemError& e) {
            if (e.errorno() == EACCES)
                __path.label("/");
            else
                throw;
        }
    }

    DIR* dir = opendir(__path.label().c_str());
    if (dir == 0) throw EXCEPTIONS::SystemError(errno);

    if (__do_chdir && (chdir(__path.label().c_str()) != 0)) {
        int sav_errno = errno;
        (void)closedir(dir);
        throw EXCEPTIONS::SystemError(sav_errno);
    }

    std::string _base(__path.label());
    assert(_base.length() > 0);

    // If _base.length()==1, then it is the root directory and we
    // don't append a slash
    if (_base.length() > 1) _base += "/";

    dirent* dent;
    while (errno = 0, dent = readdir(dir)) {
        std::string _tmp(_base + dent->d_name);

        struct stat _stat;
        if (stat(_tmp.c_str(), &_stat) == -1) {
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

    __directories.set(_dirs);
    // If the file mode on the directory is only read for the user,
    // e.g. 444, and we don't do a chdir(), we end up with an empty
    // directory list. So, make sure at least `.' and `..' is in the
    // list.
    if (__directories.empty()) {
        __directories.add(".");
        __directories.add("..");
    }

    __files.set(_files);

    if (errno_save != 0) {
        (void)closedir(dir);
        throw EXCEPTIONS::SystemError(errno_save);
    }

    if (closedir(dir) != 0) throw EXCEPTIONS::SystemError(errno);
}

void FileDialog::listbox_enter_handler(Event& _e) {
    assert(_e == EVT_LISTBOX_ENTER);

    EventEx<ListBox<>*>& _evt = dynamic_cast<EventEx<ListBox<>*>&>(_e);

    if (&__directories == _evt.data()) {
        if (__directories.selected() == ".") return;

        if (__directories.selected() == "..") {
            __path.label(dir_up(__path.label()));
        } else {
            // If __path.label().length()==1, then it is the root
            // directory and we don't append a slash
            if (__path.label().length() > 1)
                __path.label(__path.label() + "/" + __directories.selected());
            else
                __path.label(__path.label() + __directories.selected());
        }
        try {
            read_dir();
        } catch (EXCEPTIONS::SystemError& ex) {
            std::string _tmp(_("Cannot change to ") + __path.label() + ":");
            __errmsgbox =
                new MessageBox2(_("System Error"), _tmp, ex.what(), OK_ONLY);
            __errmsgbox->show();

            // Most likely the directory is not accessible, so go
            // one up. If we happen to reach the root directory,
            // and that's not accessible too, the system probably
            // is messed up...
            __path.label(dir_up(__path.label()));
        }
        return;
    }

    if (&__files == _evt.data()) {
        __filename.input(__files.selected());
    }
}

void FileDialog::filename_readonly(bool _ro) { __filename.readonly(_ro); }

std::string FileDialog::getcwd() const {
    long path_max;
    if (__path.label().empty()) {
        path_max = pathconf("/", _PC_PATH_MAX);
    } else {
        path_max = pathconf(__path.label().c_str(), _PC_PATH_MAX);
    }

    if (path_max < 0) {
        // probably something went wrong in call to pathconf(). Use
        // the fallback size.
        path_max = DEFCWDBUFSZ;
    }

    char* cwd = new char[path_max];

    char* ptr = ::getcwd(cwd, path_max);
    if (ptr == 0) {
        delete[] cwd;
        throw EXCEPTIONS::SystemError(errno);
    }

    std::string retval(ptr);
    delete[] cwd;

    return retval;
}

void FileDialog::window_close_handler(Event& _e) {
    assert(_e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(_e);

    if (__filetypemismatch == _evt.data()) {
        delete __filetypemismatch;
        __filetypemismatch = 0;
        return;
    }

    if (__errmsgbox == _evt.data()) {
        delete __errmsgbox;
        __errmsgbox = 0;
        return;
    }
}

void FileDialog::button_press_handler(Event& _e) {
    // We hook in the button_press_handler of Dialog, so we can test
    // if the file selected matches the required criteria

    if (realization() != REALIZED) return;

    // If any file type is ok, call Dialog's button press handler
    // immediately and return, since there is no work for us.
    if (__sel_type == ANY) {
        Dialog::button_press_handler(_e);
        return;
    }

    assert(_e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(_e);

    // make sure the event is intended for us
    if (evt.data() != ok_button() && evt.data() != yes_button() &&
        evt.data() != cancel_button() && evt.data() != no_button())
        return;

    // only intercept ok or yes button, i.e. ignore cancel or no
    // button.
    if (evt.data() != ok_button() && evt.data() != yes_button()) {
        Dialog::button_press_handler(_e);
        return;
    }

    if (!selection_type_match()) {
        // the selection did not match the requested selection
        // type. The method has raised a dialog to indicate the
        // missmatch to the user. So we have to return and do nothing.
        return;
    }

    Dialog::button_press_handler(_e);
}

bool FileDialog::selection_type_match() {
    struct stat statbuf;
    if (stat(filepath().c_str(), &statbuf) != 0) {
        // there is an error, it might be, that the selected file does
        // not exist. In that case, we can't make any assertion about
        // the file type and assume that it matches the selection
        // criteria.
        if (errno == ENOENT) {
            // ok, no such file
            return true;
        }

        // some other error
        //
        // get error message by using the SystemError exception
        // which does the job for us...
        EXCEPTIONS::SystemError _err(errno);
        // ... and display it
        assert(__errmsgbox == 0);
        __errmsgbox = new MessageBox2(_("System Error"), filepath(),
                                      _err.what(), OK_ONLY);
        __errmsgbox->show();
        return false;
    }

    // no error, let's see if the selection matches the expectation
    switch (__sel_type) {
        case FILE:
            if (S_ISREG(statbuf.st_mode) == 0) {
                assert(__filetypemismatch == 0);
                __filetypemismatch = new MessageBox2(
                    _("Error"), filepath(), _("is not a file"), OK_ONLY);
                __filetypemismatch->show();
                return false;
            }
            break;
        case DIRECTORY:
            if (S_ISDIR(statbuf.st_mode) == 0) {
                assert(__filetypemismatch == 0);
                __filetypemismatch = new MessageBox2(
                    _("Error"), filepath(), _("is not a directory"), OK_ONLY);
                __filetypemismatch->show();
                return false;
            }
            break;
        default:
            throw std::invalid_argument(
                _("Selection type unknown for FileDialog"));
            break;
    }

    return true;
}

//
// Public
//

FileDialog::FileDialog(const std::string& title, std::string _path,
                       bool _do_chdir, DIALOG_TYPE _dt)
    : Dialog(title, _dt, FULLSIZE),
      __errmsgbox(0),
      __filetypemismatch(0),
      __path(_path),
      __directories(),
      __files(),
      __filename(),
      __hpack(),
      __vpack(),
      __do_chdir(_do_chdir),
      __sel_type(ANY),
      __suffix() {
    __path.color(DIALOG);
    // Make sure the path specified by the user does not have a
    // trailing slash
    if (!__path.label().empty()) {
        std::string tmp = cleanup_path(__path.label());
        // is it the root directory, if so, don't touch it, if not,
        // see if there is a trailing slash
        if (tmp.length() == 1 && tmp[0] == '/') {
            __path.label(tmp);
        } else {  // ok, see if we have to remove a trailing slash
            if (tmp[tmp.length() - 1] == '/')
                __path.label(tmp.substr(0, tmp.length() - 2));
            else
                __path.label(tmp);
        }
    }

    __directories.sort_order(ASCENDING);

    __files.sort_order(ASCENDING);

    __hpack.add_back(&__directories);
    __hpack.add_back(&__files);

    __hpack.always_dynamic(true);
    __hpack.hinting(false);

    __vpack.add_back(&__path);
    __vpack.add_back(&__hpack);

    __vpack.add_back(&__filename);

    __vpack.always_dynamic(true);
    __vpack.hinting(false);

    widget(&__vpack);

    EventQueue::connect_event(EventConnectorMethod1<FileDialog>(
        EVT_LISTBOX_ENTER, this, &FileDialog::listbox_enter_handler));
    EventQueue::connect_event(EventConnectorMethod1<FileDialog>(
        EVT_WINDOW_CLOSE, this, &FileDialog::window_close_handler));
}

FileDialog::~FileDialog() {
    EventQueue::disconnect_event(EventConnectorMethod1<FileDialog>(
        EVT_LISTBOX_ENTER, this, &FileDialog::listbox_enter_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<FileDialog>(
        EVT_WINDOW_CLOSE, this, &FileDialog::window_close_handler));

    if (__errmsgbox) delete (__errmsgbox);
    if (__filetypemismatch) delete (__filetypemismatch);
}

std::string FileDialog::filepath() const {
    std::string retval;

    if (__path.label() == "/")
        retval = __path.label() + filename();
    else
        retval = __path.label() + "/" + filename();

    return retval;
}

const std::string& FileDialog::directory() const { return __path.label(); }

std::string FileDialog::filename() const {
    if (__sel_type == FILE && !__suffix.empty()) {
        if (__filename.input().length() < __suffix.length())
            return __filename.input() + __suffix;

        if (__filename.input().substr(__filename.input().length() -
                                      __suffix.length()) != __suffix)
            return __filename.input() + __suffix;

        return __filename.input();
    } else {
        return __filename.input();
    }
}

void FileDialog::do_chdir(bool _v) { __do_chdir = _v; }

bool FileDialog::do_chdir() const { return __do_chdir; }

void FileDialog::selection_type(FILEDIALOG_SELECTION_TYPE _t) {
    __sel_type = _t;
}

FILEDIALOG_SELECTION_TYPE
FileDialog::selection_type() const { return __sel_type; }

void FileDialog::suffix(const std::string& _s) { __suffix = _s; }

void FileDialog::refresh(bool immediate) {
    Dialog::refresh(immediate);

    try {
        read_dir();
    } catch (EXCEPTIONS::SystemError& ex) {
        std::string _tmp(_("Cannot change to ") + __path.label() + ":");
        __errmsgbox =
            new MessageBox2(_("System Error"), _tmp, ex.what(), OK_ONLY);
        __errmsgbox->show();

        // Most likely the directory is not accessible, so go one
        // up. If we happen to reach the root directory, and
        // that's not accessible too, the system probably is
        // messed up...
        __path.label(dir_up(__path.label()));
    }
}
