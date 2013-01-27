// -*- mode: c++ -*-

#ifndef WIDGET_H
#define WIDGET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>

#include "mycurses.h"
#include "widgetbase.h"


/**
 * Implements a widget.
 *
 */
class Widget: public WidgetBase {
    private:
	/**
	 * Keeps track of how many instance objects have been created
	 * sharing the same WINDOW structure
	 */
	unsigned int* __instance_count;
	/**
	 * Pointer to a pointer so that we can replace the window for
	 * all instances simultaneously.
	 */
	WINDOW** __widget_subwin;

    protected:
	int force_refresh_handler(Event& _e);

	void unrealize();

	WINDOW* widget_subwin() const;

    public:
	Widget();
	Widget(const Widget& _w);
	~Widget();
	Widget& operator=(const Widget& _w);

	// Must be overriden in derived classes
	//bool size_change();

	// Inherited from Realizeable
	void refresh(bool immediate);
	void resize(const Area& _a);
	void realize();

};


#endif // WIDGET_H
