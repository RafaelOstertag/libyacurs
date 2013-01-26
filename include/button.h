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
	 *
	 * It visibly change the focus.
	 */
	void visibly_change_focus();

    protected:
	/**
	 * Will be called upon key press.
	 *
	 * Derived Buttons have to implement this.
	 */
	virtual void on_press() = 0;

	virtual int key_handler(Event& _e);

	virtual void realize();
	virtual void unrealize();


    public:
	Button(const std::string& _b);
	Button(const Button& _b);
	~Button();
	Button& operator=(const Button& _b);

	bool can_focus() const;
	void focus(bool _f);
	bool focus() const;
};

#endif // BUTTON_H
