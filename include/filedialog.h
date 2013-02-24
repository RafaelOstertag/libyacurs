// -*- mode: c++ -*-
//
// $Id$

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include <unistd.h>

#include "dialog.h"
#include "dynlabel.h"
#include "input.h"
#include "listbox.h"
#include "messagebox2.h"

/**
 */
class FileDialog: public Dialog {
    private:
	MessageBox2* __msgbox;
	DynLabel* __path;
	ListBox<>* __directories;
	ListBox<>* __files;
	Input* __filename;
	HPack* __hpack;
	VPack* __vpack;

	// Not supported
	FileDialog& operator=(const FileDialog&);

	std::string dir_up(const std::string& dir);
	void read_dir();

	void listbox_enter_handler(Event& _e);
	void window_close_handler(Event& _e);
    public:
	FileDialog(std::string _path=std::string(),
		   DIALOG_TYPE _dt=OKCANCEL);

	virtual ~FileDialog();

	std::string filepath() const;
	const std::string& directory() const;
	const std::string& filename() const;

	void refresh(bool immediate);
};

#endif // FILEDIALOG_H
