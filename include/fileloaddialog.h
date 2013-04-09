// -*- mode: c++ -*-
//
// $Id$

#ifndef FILELOADDIALOG_H
#define FILELOADDIALOG_H

#include <string>

#include <unistd.h>

#include "dialog.h"
#include "dynlabel.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

namespace YACURS {
    class FileLoadDialog: public Dialog {
	private:
	    MessageBox2* __msgbox;
	    DynLabel* __path;
	    ListBox<>* __directories;
	    ListBox<>* __files;
	    Input<>* __filename;
	    HPack* __hpack;
	    VPack* __vpack;

	    // Not supported
	    FileLoadDialog& operator=(const FileLoadDialog&);

	    std::string dir_up(const std::string& dir);
	    void read_dir();

	    void listbox_enter_handler(Event& _e);
	    void window_close_handler(Event& _e);
	public:
	    FileLoadDialog(std::string _path=std::string(),
		       DIALOG_TYPE _dt=OKCANCEL);

	    virtual ~FileLoadDialog();

	    std::string filepath() const;
	    const std::string& directory() const;
	    const std::string& filename() const;

	    void refresh(bool immediate);
    };
}

#endif // FILELOADDIALOG_H
