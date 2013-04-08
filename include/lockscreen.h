// -*- mode: c++ -*-
//
// $Id$

#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "window.h"
#include "unlockdialog.h"
#include "messagebox.h"

namespace YACURS {
    class LockScreen: public Window {
	private:
	    unsigned int __timeout;
	    unsigned int __unlock_diag_timeout;
	    UnlockDialog* __unlock_dialog;
	    MessageBox* __msgbox;
	    // Not supported
	    LockScreen& operator=(const LockScreen&);

	protected:
	    virtual void event_key_handler(Event& _e);
	    virtual void event_window_close_handler(Event& _e);

	public:
	    LockScreen(UnlockDialog* _unlock, unsigned int timeout, unsigned int ulck_timeout);
	    virtual ~LockScreen();

	    unsigned int timeout() const;
	    unsigned int unlock_dialog_timeout() const;

	    virtual void close_unlock_dialog();

	    // From WindowBase
	    void show();
	    void close();
    };
}

#endif // LOCKSCREEN_H
