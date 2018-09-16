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
    std::list<std::string> dirs;
    std::list<std::string> files;

    if (_path.label().empty()) {
        try {
            _path.label(getcwd());
        } catch (EXCEPTIONS::SystemError& e) {
            if (e.errorno() == EACCES)
                _path.label("/");
            else
                throw;
        }
    }

    DIR* dir = opendir(_path.label().c_str());
    if (dir == 0) throw EXCEPTIONS::SystemError(errno);

    if (_do_chdir && (chdir(_path.label().c_str()) != 0)) {
        int sav_errno = errno;
        (void)closedir(dir);
        throw EXCEPTIONS::SystemError(sav_errno);
    }

    std::string _base(_path.label());
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
            dirs.push_back(dent->d_name);
            continue;
        }
        if (_stat.st_mode & S_IFREG) {
            files.push_back(dent->d_name);
            continue;
        }
    }

    // On Fedora 18, somewhere in _directories->set(dirs), errno
    // will be updated, so we save it here, before proceeding
    int errno_save = errno;

    _directories.set(dirs);
    // If the file mode on the directory is only read for the user,
    // e.g. 444, and we don't do a chdir(), we end up with an empty
    // directory list. So, make sure at least `.' and `..' is in the
    // list.
    if (_directories.empty()) {
        _directories.add(".");
        _directories.add("..");
    }

    _files.set(files);

    if (errno_save != 0) {
        (void)closedir(dir);
        throw EXCEPTIONS::SystemError(errno_save);
    }

    if (closedir(dir) != 0) throw EXCEPTIONS::SystemError(errno);
}

void FileDialog::listbox_enter_handler(Event& e) {
    assert(e == EVT_LISTBOX_ENTER);

    EventEx<ListBox<>*>& _evt = dynamic_cast<EventEx<ListBox<>*>&>(e);

    if (&_directories == _evt.data()) {
        if (_directories.selected() == ".") return;

        if (_directories.selected() == "..") {
            _path.label(dir_up(_path.label()));
        } else {
            // If _path.label().length()==1, then it is the root
            // directory and we don't append a slash
            if (_path.label().length() > 1)
                _path.label(_path.label() + "/" + _directories.selected());
            else
                _path.label(_path.label() + _directories.selected());
        }
        try {
            read_dir();
        } catch (EXCEPTIONS::SystemError& ex) {
            std::string _tmp(_("Cannot change to ") + _path.label() + ":");
            _errmsgbox =
                new MessageBox2(_("System Error"), _tmp, ex.what(), OK_ONLY);
            _errmsgbox->show();

            // Most likely the directory is not accessible, so go
            // one up. If we happen to reach the root directory,
            // and that's not accessible too, the system probably
            // is messed up...
            _path.label(dir_up(_path.label()));
        }
        return;
    }

    if (&_files == _evt.data()) {
        _filename.input(_files.selected());
    }
}

void FileDialog::filename_readonly(bool ro) { _filename.readonly(ro); }

std::string FileDialog::getcwd() const {
    long path_max;
    if (_path.label().empty()) {
        path_max = pathconf("/", _PC_PATH_MAX);
    } else {
        path_max = pathconf(_path.label().c_str(), _PC_PATH_MAX);
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

void FileDialog::window_close_handler(Event& e) {
    assert(e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(e);

    if (_filetypemismatch == _evt.data()) {
        delete _filetypemismatch;
        _filetypemismatch = 0;
        return;
    }

    if (_errmsgbox == _evt.data()) {
        delete _errmsgbox;
        _errmsgbox = 0;
        return;
    }
}

void FileDialog::button_press_handler(Event& e) {
    // We hook in the button_press_handler of Dialog, so we can test
    // if the file selected matches the required criteria

    if (realization() != REALIZED) return;

    // If any file type is ok, call Dialog's button press handler
    // immediately and return, since there is no work for us.
    if (_sel_type == ANY) {
        Dialog::button_press_handler(e);
        return;
    }

    assert(e == EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(e);

    // make sure the event is intended for us
    if (evt.data() != ok_button() && evt.data() != yes_button() &&
        evt.data() != cancel_button() && evt.data() != no_button())
        return;

    // only intercept ok or yes button, i.e. ignore cancel or no
    // button.
    if (evt.data() != ok_button() && evt.data() != yes_button()) {
        Dialog::button_press_handler(e);
        return;
    }

    if (!selection_type_match()) {
        // the selection did not match the requested selection
        // type. The method has raised a dialog to indicate the
        // missmatch to the user. So we have to return and do nothing.
        return;
    }

    Dialog::button_press_handler(e);
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
        assert(_errmsgbox == 0);
        _errmsgbox = new MessageBox2(_("System Error"), filepath(), _err.what(),
                                     OK_ONLY);
        _errmsgbox->show();
        return false;
    }

    // no error, let's see if the selection matches the expectation
    switch (_sel_type) {
        case FILE:
            if (S_ISREG(statbuf.st_mode) == 0) {
                assert(_filetypemismatch == 0);
                _filetypemismatch = new MessageBox2(
                    _("Error"), filepath(), _("is not a file"), OK_ONLY);
                _filetypemismatch->show();
                return false;
            }
            break;
        case DIRECTORY:
            if (S_ISDIR(statbuf.st_mode) == 0) {
                assert(_filetypemismatch == 0);
                _filetypemismatch = new MessageBox2(
                    _("Error"), filepath(), _("is not a directory"), OK_ONLY);
                _filetypemismatch->show();
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

FileDialog::FileDialog(const std::string& title, std::string path,
                       bool do_chdir, DIALOG_TYPE dt)
    : Dialog(title, dt, FULLSIZE),
      _errmsgbox(0),
      _filetypemismatch(0),
      _path(path),
      _directories(),
      _files(),
      _filename(),
      _hpack(),
      _vpack(),
      _do_chdir(do_chdir),
      _sel_type(ANY),
      _suffix() {
    _path.color(DIALOG);
    // Make sure the path specified by the user does not have a
    // trailing slash
    if (!_path.label().empty()) {
        std::string tmp = cleanup_path(_path.label());
        // is it the root directory, if so, don't touch it, if not,
        // see if there is a trailing slash
        if (tmp.length() == 1 && tmp[0] == '/') {
            _path.label(tmp);
        } else {  // ok, see if we have to remove a trailing slash
            if (tmp[tmp.length() - 1] == '/')
                _path.label(tmp.substr(0, tmp.length() - 2));
            else
                _path.label(tmp);
        }
    }

    _directories.sort_order(ASCENDING);

    _files.sort_order(ASCENDING);

    _hpack.add_back(&_directories);
    _hpack.add_back(&_files);

    _hpack.always_dynamic(true);
    _hpack.hinting(false);

    _vpack.add_back(&_path);
    _vpack.add_back(&_hpack);

    _vpack.add_back(&_filename);

    _vpack.always_dynamic(true);
    _vpack.hinting(false);

    widget(&_vpack);

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

    if (_errmsgbox) delete (_errmsgbox);
    if (_filetypemismatch) delete (_filetypemismatch);
}

std::string FileDialog::filepath() const {
    std::string retval;

    if (_path.label() == "/")
        retval = _path.label() + filename();
    else
        retval = _path.label() + "/" + filename();

    return retval;
}

const std::string& FileDialog::directory() const { return _path.label(); }

std::string FileDialog::filename() const {
    if (_sel_type == FILE && !_suffix.empty()) {
        if (_filename.input().length() < _suffix.length())
            return _filename.input() + _suffix;

        if (_filename.input().substr(_filename.input().length() -
                                     _suffix.length()) != _suffix)
            return _filename.input() + _suffix;

        return _filename.input();
    } else {
        return _filename.input();
    }
}

void FileDialog::do_chdir(bool v) { _do_chdir = v; }

bool FileDialog::do_chdir() const { return _do_chdir; }

void FileDialog::selection_type(FILEDIALOG_SELECTION_TYPE t) { _sel_type = t; }

FILEDIALOG_SELECTION_TYPE
FileDialog::selection_type() const { return _sel_type; }

void FileDialog::suffix(const std::string& s) { _suffix = s; }

void FileDialog::refresh(bool immediate) {
    Dialog::refresh(immediate);

    try {
        read_dir();
    } catch (EXCEPTIONS::SystemError& ex) {
        std::string _tmp(_("Cannot change to ") + _path.label() + ":");
        _errmsgbox =
            new MessageBox2(_("System Error"), _tmp, ex.what(), OK_ONLY);
        _errmsgbox->show();

        // Most likely the directory is not accessible, so go one
        // up. If we happen to reach the root directory, and
        // that's not accessible too, the system probably is
        // messed up...
        _path.label(dir_up(_path.label()));
    }
}
