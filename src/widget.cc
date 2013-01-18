// $Id$

#include <cassert>

#include "cursex.h"
#include "widget.h"

//
// Private
//

//
// Protected
//
void
Widget::unrealize() {
    if (not realized()) throw NotRealized();

    realized(false);

    assert(__subwin!=NULL);
    assert(*__subwin!=NULL);

    if (delwin(*__subwin) == ERR)
	throw DelWindowFailed();

    *__subwin = NULL;
}

//
// Public
//

Widget::Widget():
    WidgetBase(), instances(NULL), __subwin(NULL) {
    __subwin = new WINDOW*;
    *__subwin = NULL;

    instances = new unsigned int;
    *instances = 1;
}

Widget::Widget(const Widget& _w):
    WidgetBase(_w), instances(_w.instances),
    __subwin(_w.__subwin) {
    (*instances)++;
}

Widget::~Widget() {
    assert(instances!=NULL);
    if (*instances > 1) {
	(*instances)--;
	return;
    }

    delete instances;

    assert(__subwin!=NULL);

    if (*__subwin != NULL)
	if (delwin(*__subwin) == ERR)
	    throw DelWindowFailed();

    delete __subwin;
}

Widget&
Widget::operator=(const Widget& _w) {
    if (this == &_w)
	return *this;

    WidgetBase::operator=(_w);

    instances = _w.instances;
    assert(instances!=NULL);
    (*instances)++;

    __subwin = _w.__subwin;

    return *this;
}

    
void
Widget::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    assert(__subwin!=NULL);
    assert(*__subwin!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*__subwin);
    else
	retval = wnoutrefresh(*__subwin);

    if (retval == ERR)
	throw RefreshFailed();
}
	
void
Widget::resize(const Area& _a) {
    //
    // 1. Keep in mind: a resize does not refresh!
    //
    // 2. The actual resize has to be done in a derived class
    //
    if (!realized()) throw NotRealized();

    assert(_a.x()>-1);
    assert(_a.y()>-1);
    assert(_a.rows()>0);
    assert(_a.cols()>0);

    unrealize();

    position(_a);
    size_available(_a);

    realize();
}

void
Widget::realize() {
    if (realized()) throw AlreadyRealized();

    const Coordinates& pos = position();
    const Size& _size = size();

    assert(curseswindow()!=NULL);
    assert(__subwin!=NULL);
    assert(*__subwin==NULL);
    *__subwin = derwin(curseswindow(),
		       _size.rows(),
		       _size.cols(),
		       pos.y(),
		       pos.x());
    if (*__subwin == NULL) {
	throw DerWinFailed();
    }

    realized(true);
}
