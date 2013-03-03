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
#include "inputbox.h"

class LockScreen: public Window {
    private:
	InputBox* __unlock_dialog;
	// Not supported
	LockScreen& operator=(const LockScreen&);

    protected:
	virtual void event_key_handler(Event& _e);
	virtual void event_window_close_handler(Event& _e);

    public:
	LockScreen(InputBox* _unlock);
	virtual ~LockScreen();

	// From WindowBase
	void show();
	void close();
};

#endif // LOCKSCREEN_H
