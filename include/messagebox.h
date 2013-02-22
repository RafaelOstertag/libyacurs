// -*- mode: c++ -*-
//
// $Id: dialog.h 4856 2013-02-15 12:29:33Z rafisol $

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "dialog.h"
#include "label.h"

/**
 */
class MessageBox: public Dialog {
    private:
	Label* __message;

	// Not supported
	MessageBox& operator=(const MessageBox&);
    public:
	MessageBox(const std::string& _title,
		   const std::string& _message,
		   DIALOG_TYPE _dt=OKCANCEL);

	virtual ~MessageBox();
};

#endif // MESSAGEBOX_H