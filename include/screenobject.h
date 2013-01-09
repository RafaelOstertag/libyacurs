// -*- mode: c++ -*-
//
// $Id$

#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mycurses.h"
#include "realizeable.h"
#include "margin.h"
#include "event.h"

class ScreenObject : public Realizeable {
    private:
	Rectangle<> rect;
	Margin<> margin;
	/// Keeps track of how many instance objects have been created sharing
	/// the same WINDOW structure
	unsigned int* instances;
	/// Pointer to a pointer so that we can replace the window for all
	/// instances simultaneously.
	WINDOW** w;

    protected:
	WINDOW* getWindow() const;

	/// Keep this. Used by tests/windowrefs.cc.
	unsigned int getInstanceCount() const;

    public:
	ScreenObject(const Margin<> _m = Margin<>());
	ScreenObject(const ScreenObject& so);
	virtual ~ScreenObject();
	ScreenObject& operator=(const ScreenObject& so);

	void refresh();
	void resize(const Rectangle<>& r);
	void realize(const Rectangle<>& r);
};

#endif // SCREENOBJECT_H
