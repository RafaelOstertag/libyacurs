// $Id$

#include <cstdlib>
#include <cassert>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "filedialog.h"

using namespace YACURS;

//
// Private
//
FileDialog&
FileDialog::operator=(const FileDialog&) {
    throw NotSupported();
    return *this;
}

std::string
FileDialog::dir_up(const std::string& dir) {
    std::string::size_type pos=dir.rfind('/');

    std::string retval;
    
    if (pos!=0) {
	retval=dir.substr(0,pos);
    } else {
	// pos==0 is the root directory, which we want
	// to preserve
	retval="/";
    }

    return retval;
}

void
FileDialog::read_dir() {
    std::list<std::string> _dirs;
    std::list<std::string> _files;

    if (__path->label().empty()) {
	char *ptr=getcwd(0, 2048);
	if (!ptr) {
	    if (errno==EACCES)
		__path->label("/");
	    else
		throw SystemError(errno);
	}
	__path->label(ptr);
    }

    DIR* dir=opendir(__path->label().c_str());
    if (dir==0)
	throw SystemError(errno);

    std::string _base(__path->label());
    assert(_base.length()>0);

    // If _base.length()==1, then it is the root directory and we
    // don't append a slash
    if (_base.length()>1)
	_base+="/";

    dirent* dent = 0;
    errno=0;
    while((dent=readdir(dir))) {
	std::string _tmp(_base + dent->d_name);

	struct stat _stat;
	if (stat(_tmp.c_str(), &_stat) == -1) {
	    // reset errno
	    errno=0;
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

    __directories->set(_dirs);
    __files->set(_files);

    if (errno!=0)
	throw SystemError(errno);


    if (closedir(dir)!=0)
	throw SystemError(errno);
}

void
FileDialog::listbox_enter_handler(Event& _e) {
    assert(_e==EVT_LISTBOX_ENTER);

    EventEx<ListBox<>*>& _evt = dynamic_cast<EventEx<ListBox<>*>&>(_e);

    if (__directories == _evt.data()) {
	if ( __directories->selected() == ".")
	    return;

	if (__directories->selected() == ".." ) {
	    __path->label(dir_up(__path->label()));
	} else {
	    // If __path->label().length()==1, then it is the root
	    // directory and we don't append a slash
	    if (__path->label().length()>1)
		__path->label(__path->label() + "/" +
			      __directories->selected());
	    else
		__path->label(__path->label() +
			      __directories->selected());

	}
	try {
	    read_dir();
	} catch (SystemError& ex) {
	    std::string _tmp("Cannot change to" + __path->label() + ":");
	    __msgbox=new MessageBox2("System Error",
				     _tmp,
				     ex.what(),
				     OK_ONLY);
	    __msgbox->show();

	    // Most likely the directory is not accessible, so go
	    // one up. If we happen to reach the root directory,
	    // and that's not accessible too, the system probably
	    // is messed up...
	    __path->label(dir_up(__path->label()));
	}
	return;
    }

    if (__files == _evt.data()) {
	__filename->input(__files->selected());
    }
}

void
FileDialog::window_close_handler(Event& _e) {
    assert(_e==EVT_WINDOW_CLOSE);
    
    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(_e);

    if (__msgbox!=0 && __msgbox==_evt.data()) {
	__msgbox->close();
	delete __msgbox;
	__msgbox=0;
    }
}
    
//
// Protected
//

//
// Public
//

FileDialog::FileDialog(std::string _path,
		       DIALOG_TYPE _dt): Dialog("File", _dt, Dialog::FULLSIZE),
					 __msgbox(0),
					 __path(0),
					 __directories(0),
					 __files(0),
					 __filename(0),
					 __hpack(0),
					 __vpack(0) {
    __vpack = new VPack;
    __hpack = new HPack;

    __path = new DynLabel(_path);

    __directories = new ListBox<>;
    __directories->sort_order(ListBox<>::ASCENDING);
    
    __files = new ListBox<>;
    __files->sort_order(ListBox<>::ASCENDING);

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

    EventQueue::connect_event(EventConnectorMethod1<FileDialog>(EVT_LISTBOX_ENTER,this,&FileDialog::listbox_enter_handler));
    EventQueue::connect_event(EventConnectorMethod1<FileDialog>(EVT_WINDOW_CLOSE, this, &FileDialog::window_close_handler));
}

FileDialog::~FileDialog() {
    EventQueue::disconnect_event(EventConnectorMethod1<FileDialog>(EVT_LISTBOX_ENTER,this,&FileDialog::listbox_enter_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<FileDialog>(EVT_WINDOW_CLOSE, this, &FileDialog::window_close_handler));

    assert(__path!=0);
    assert(__directories!=0);
    assert(__files!=0);
    assert(__filename!=0);
    assert(__hpack!=0);
    assert(__vpack!=0);

    delete __path;
    delete __directories;
    delete __files;
    delete __filename;
    delete __hpack;
    delete __vpack;
}

std::string
FileDialog::filepath() const {
    assert(__path!=0);
    assert(__directories!=0);
    assert(__files!=0);
    assert(__filename!=0);

    std::string retval;

    if (__path->label()=="/")
	retval=__path->label()+__filename->input();
    else
	retval=__path->label()+"/"+__filename->input();

    return retval;
}

const std::string&
FileDialog::directory() const {
    assert(__path!=0);
    assert(__directories!=0);
    assert(__files!=0);
    assert(__filename!=0);

    return __path->label();
}

const std::string&
FileDialog::filename() const {
    return __filename->input();
}

void
FileDialog::refresh(bool immediate) {
    Dialog::refresh(immediate);

    try {
	read_dir();
    } catch (SystemError& ex) {
	std::string _tmp("Cannot change to" + __path->label() + ":");
	__msgbox=new MessageBox2("System Error",
				_tmp,
				ex.what(),
				OK_ONLY);
	__msgbox->show();
	
	// Most likely the directory is not accessible, so go one
	// up. If we happen to reach the root directory, and
	// that's not accessible too, the system probably is
	// messed up...
	__path->label(dir_up(__path->label()));
    }
}
