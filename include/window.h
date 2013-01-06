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
	Margin<> margin;
	bool hasframe;

	void __init();

    public:
	Window();
	Window(const Margin<>& m);
	Window(const Window& W);
	Window& operator=(const Window& W);
	bool operator==(const Window& W) const;
	bool getFrame() const;
	void setFrame(bool b);

	void realize(const Rectangle<>& r);
};

#endif // WINDOWS_H
