// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOW_H
#define WINDOW_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "mycurses.h"
#include "windowbase.h"
#include "widgetbase.h"

// Forward declaration because widgetbase.h already included window.h
class WidgetBase;

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
	void resize(const Area& _a);
	void realize();
};

#endif // WINDOW_H
