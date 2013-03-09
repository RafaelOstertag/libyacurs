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

class LockScreen: public Window {
    private:
	UnlockDialog* __unlock_dialog;
	MessageBox* __msgbox;
	// Not supported
	LockScreen& operator=(const LockScreen&);

    protected:
	virtual void event_key_handler(Event& _e);
	virtual void event_window_close_handler(Event& _e);

    public:
	LockScreen(UnlockDialog* _unlock);
	virtual ~LockScreen();

	// From WindowBase
	void show();
	void close();
};

#endif // LOCKSCREEN_H
