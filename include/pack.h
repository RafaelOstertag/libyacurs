// -*- mode: c++ -*-
//
// $Id$

#ifndef PACK_H
#define PACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIST
#include <list>
#endif // HAVE_LIST

#include "rectangle.h"
#include "widget.h"

class Pack: public Widget {
    protected:
	std::list<Widget*> widget_list;
	Rectangle __size;

    public:
	Pack();
	void add(Widget* _w);
	void remove(Widget* _w);
	void size(const Rectangle&);
	const Rectangle& size();

};

#endif // PACK_H
