// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOWS_H
#define WINDOWS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "screenobject.h"
#include "margin.h"
#include "cursex.h"

#include "mycurses.h"


class Window: public ScreenObject {
    private:
	bool hasframe;

    protected:
	virtual int refresh_handler(Event& _e);
	virtual int resize_handler(Event& _e);
	virtual int key_handler(Event& _e);

    public:
	Window();
	Window(const Margin& m);
	Window(const Window& W);
	~Window();
	Window& operator=(const Window& W);

	bool frame() const;
	void frame(bool b);

	void realize();
};

#endif // WINDOWS_H
