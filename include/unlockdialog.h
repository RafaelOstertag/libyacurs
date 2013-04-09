// -*- mode: c++ -*-
//
// $Id$

#ifndef UNLOCKDIALOG_H
#define UNLOCKDIALOG_H

#include <string>

#include "dialog.h"

namespace YACURS {
    class UnlockDialog: public Dialog {
	private:
	    UnlockDialog& operator=(const UnlockDialog&);
	public:
	    UnlockDialog(const std::string& _title);
	    virtual ~UnlockDialog();

	    // From Dialog
	    void realize();
	
	    /**
	     * @return @c true if unlock should happen, @c false
	     * otherwise.
	     */
	    virtual bool unlock() = 0;

	    virtual void clear() = 0;
    };
}

#endif // UNLOCKDIALOG_H
