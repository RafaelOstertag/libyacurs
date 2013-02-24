// -*- mode: c++ -*-
//
// $Id$

#ifndef DIALOG_H
#define DIALOG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "window.h"
#include "vpack.h"
#include "hpack.h"
#include "button.h"

/**
 */
class Dialog: public Window {
    public:
	enum STATE {
	    DIALOG_OK,
	    DIALOG_CANCEL
	};

	enum DIALOG_TYPE {
	    OK_ONLY,
	    YES_ONLY,
	    OKCANCEL,
	    YESNO
	};

	enum DIALOG_SIZE {
	    AUTOMATIC,
	    FULLSIZE
	};

    private:
	VPack* __vpack;
	HPack* __hpack;
	Button* __bok;
	Button* __bcancel;
	STATE __dstate;
        DIALOG_TYPE __dialog_type;
	DIALOG_SIZE __dialog_size;
	

	std::string __title;

	// Not supported
	Dialog(const Dialog&);
	Dialog& operator=(const Dialog&);
    protected:
	virtual void button_press_handler(Event& _e);

	void unrealize();


    public:
	Dialog(const std::string& _title, 
	       DIALOG_TYPE _dt=OKCANCEL,
	       DIALOG_SIZE _ds=AUTOMATIC);
	virtual ~Dialog();
	STATE dialog_state() const;

	void widget(WidgetBase* _w);
	
	// Those are from Realizable
	void refresh(bool immediate);
	// Does nothing, everything handled in parent.
	//void resize(const Area& _a);
	void realize();
};

#endif // DIALOG_H
