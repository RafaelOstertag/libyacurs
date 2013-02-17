// -*- mode: c++ -*-
//
// $Id$

#ifndef INPUTBOX_H
#define INPUTBOX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "dialog.h"
#include "label.h"
#include "input.h"

/**
 */
class InputBox: public Dialog {
    private:
	Label* __message;
	Input* __input;
	VPack* __vpack;

	// Not supported
	InputBox& operator=(const InputBox&);
    public:
	InputBox(const std::string& _title,
		   const std::string& _message,
		   DIALOG_TYPE _dt=OKCANCEL);

	virtual ~InputBox();

	const std::string& input() const;
};

#endif // INPUTBOX_H
