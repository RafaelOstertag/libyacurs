// -*- mode: c++ -*-
//
// $Id: dialog.h 4883 2013-02-24 10:40:05Z rafisol $

#ifndef UNLOCKDIADEFAULT_H
#define UNLOCKDIADEFAULT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "unlockdialog.h"
#include "vpack.h"
#include "label.h"
#include "input.h"

namespace YACURS {

    class UnlockDialogDefault: public UnlockDialog {
	private:
	    std::string __secret;
	    VPack* __vpack;
	    Label* __text;
	    Input<>* __secret_input;

	    UnlockDialogDefault& operator=(const UnlockDialogDefault&);

	public:
	    UnlockDialogDefault(const std::string& _secret);

	    ~UnlockDialogDefault();

	    bool unlock();
	    void clear();
    };
}

#endif // UNLOCKDIADEFAULT_H
