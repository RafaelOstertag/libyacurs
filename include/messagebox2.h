// -*- mode: c++ -*-
//
// $Id$

#ifndef MESSAGEBOX2_H
#define MESSAGEBOX2_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "dialog.h"
#include "label.h"

/**
 */
class MessageBox2: public Dialog {
    private:
	VPack* __vpack;
	Label* __message1;
	Label* __message2;

	// Not supported
	MessageBox2& operator=(const MessageBox2&);
    public:
	MessageBox2(const std::string& _title,
		    const std::string& _message1,
		    const std::string& _message2,
		    DIALOG_TYPE _dt=OKCANCEL);

	virtual ~MessageBox2();
};

#endif // MESSAGEBOX2_H
