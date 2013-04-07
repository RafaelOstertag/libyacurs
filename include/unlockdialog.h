// -*- mode: c++ -*-
//
// $Id: dialog.h 4883 2013-02-24 10:40:05Z rafisol $

#ifndef UNLOCKDIALOG_H
#define UNLOCKDIALOG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
