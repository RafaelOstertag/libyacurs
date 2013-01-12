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
	/// The dimension of screen object
	Rectangle<> rect;
	/// margin of the screen object
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

	void setMargin(const Margin<>& _m);
	const Margin<>& getMargin() const;

	void unrealize();

    public:
	/**
	 * @param _r the size of the window. If it is equal to Rectangle<>(),
	 * ScreenObject will retrieve the screen size by calling
	 * Curses::inquiryScreenSize().
	 */
	ScreenObject(const Rectangle<>& _r = Rectangle<>(),
		     const Margin<>& _m = Margin<>());
	ScreenObject(const ScreenObject& so);
	virtual ~ScreenObject();
	ScreenObject& operator=(const ScreenObject& so);

	void refresh(bool immediate);
	void resize(const Rectangle<>& _r);
	void realize();

};

#endif // SCREENOBJECT_H
