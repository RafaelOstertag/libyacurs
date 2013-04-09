// -*- mode: c++ -*-
//
// $Id$

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <string>

#include "dialog.h"
#include "label.h"

namespace YACURS {
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
}

#endif // MESSAGEBOX_H
