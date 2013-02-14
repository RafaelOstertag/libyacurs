// -*- mode: c++ -*-
//
// $Id$

#ifndef DIALOG_H
#define DIALOG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "window.h"
#include "vpack.h"
#include "hpack.h"
#include "button.h"

/**
 */
class Dialog: public Window {
    public:
	enum DIALOG_STATE {
	    DIALOG_OK,
	    DIALOG_CANCEL
	};

    private:
	VPack* __vpack;
	HPack* __hpack;
	Button* __bok;
	Button* __bcancel;
	DIALOG_STATE __dstate;
	bool __ok_only;
	

	std::string __title;

    protected:
	virtual void button_press_handler(Event& _e);

	void unrealize();
    public:
	Dialog(const std::string& _title, bool _ok_only=false);
	Dialog(const Dialog& _dialog);
	virtual ~Dialog();
	Dialog& operator=(const Dialog& _dialog);
	
	// Those are from Realizable
	void refresh(bool immediate);
	// Does nothing, everything handled in parent.
	//void resize(const Area& _a);
	void realize();
};

#endif // DIALOG_H
