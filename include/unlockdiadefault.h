// -*- mode: c++ -*-
//
// $Id$

#ifndef UNLOCKDIADEFAULT_H
#define UNLOCKDIADEFAULT_H

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
