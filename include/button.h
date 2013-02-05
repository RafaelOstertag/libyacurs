// -*- mode: c++ -*-
//
// $Id$

#ifndef BUTTON_H
#define BUTTON_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "label.h"
#include "event.h"

/**
 * A button that can be pressed.
 *
 * Derived classes must implement on_press().
 */
class Button: public Label {
    private:
	bool __focus;

	/**
	 * Visibly change focus.
	 */
	void visibly_change_focus();

    protected:
	virtual int key_handler(Event& _e);

	// From Realizeable
	virtual void realize();
	virtual void unrealize();


    public:
	Button(const std::string& _b);
	Button(const Button& _b);
	virtual ~Button();
	Button& operator=(const Button& _b);

	void label(const std::string& _l);
	std::string label() const;

	// From WidgetBase
	bool can_focus() const;
	void focus(bool _f);
	bool focus() const;

	// From Realizeable
	void refresh(bool immediate);
};

#endif // BUTTON_H
