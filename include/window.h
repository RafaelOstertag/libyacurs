// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOWS_H
#define WINDOWS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "windowbase.h"
#include "widgetbase.h"

#include "mycurses.h"


/**
 * Window for displaying a widget.
 */
class Window: public WindowBase {
    private:
	WidgetBase* __widget;

    protected:
	void unrealize();

	virtual int key_handler(Event& _e);

    public:
	Window(const Margin& m=Margin());
	Window(const Window& W);
	~Window();
	Window& operator=(const Window& W);
	
	void widget(WidgetBase* _w);
	WidgetBase* widget() const;

	// Those are from Realizable
	void refresh(bool immediate);
	void resize(const Size& _s);
	void realize();
};

#endif // WINDOWS_H
