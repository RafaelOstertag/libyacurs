// -*- mode: c++ -*-
//
// $Id$

#ifndef FILESAVEDIALOG_H
#define FILESAVEDIALOG_H

#include <string>

#include <unistd.h>

#include "dialog.h"
#include "dynlabel.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

namespace YACURS {
    class FileSaveDialog: public Dialog {
	private:
	    MessageBox2* __errmsgbox;
	    MessageBox2* __confirmdia;
	    DynLabel* __path;
	    ListBox<>* __directories;
	    ListBox<>* __files;
	    Input<>* __filename;
	    HPack* __hpack;
	    VPack* __vpack;

	    // Not supported
	    FileSaveDialog& operator=(const FileSaveDialog&);

	    std::string dir_up(const std::string& dir);
	    void read_dir();

	    void listbox_enter_handler(Event& _e);
	    void window_close_handler(Event& _e);

	protected:
	    virtual void button_press_handler(Event& _e);

	public:
	    FileSaveDialog(std::string _path=std::string(),
		       DIALOG_TYPE _dt=OKCANCEL);

	    virtual ~FileSaveDialog();

	    std::string filepath() const;
	    const std::string& directory() const;
	    const std::string& filename() const;

	    void refresh(bool immediate);
    };
}

#endif // FILESAVEDIALOG_H
