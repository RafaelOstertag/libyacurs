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

    // We have to clear the window since the new size might be
    // smaller, and thus leaving artifacts on the screen if we omit to
    // clear the entire subwin()
    if (wclear(*__subwin) == ERR)
	throw ClearFailed();

    if (delwin(*__subwin) == ERR)
	throw DelWindowFailed();
    *__subwin = NULL;

    // This is also needed to remove artifacts on the screen
    if (touchwin(curses_window()) == ERR)
	throw TouchFailed();
    if (wrefresh(curses_window()) == ERR)
	throw RefreshFailed();


}

WINDOW*
Widget::subwin() const {
    return *__subwin;
}

//
// Public
//

Widget::Widget():
    WidgetBase(), __instance_count(NULL), __subwin(NULL) {
    __subwin = new WINDOW*;
    *__subwin = NULL;

    __instance_count = new unsigned int;
    *__instance_count = 1;
}

Widget::Widget(const Widget& _w):
    WidgetBase(_w), __instance_count(_w.__instance_count),
    __subwin(_w.__subwin) {
    (*__instance_count)++;
}

Widget::~Widget() {
    assert(__instance_count!=NULL);
    if (*__instance_count > 1) {
	(*__instance_count)--;
	return;
    }

    delete __instance_count;

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

    __instance_count = _w.__instance_count;
    assert(__instance_count!=NULL);
    (*__instance_count)++;

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

    // We cannot assert on parent() since it might be legally NULL
    // assert(parent()!=NULL

    assert(size()!=Size());
    assert(size_available()!=Size());

    assert(curses_window()!=NULL);
    assert(__subwin!=NULL);
    assert(*__subwin==NULL);

    *__subwin = ::subwin(curses_window(),
		       _size.rows(),
		       _size.cols(),
		       pos.y(),
		       pos.x());
    if (*__subwin == NULL) {
	throw DerWinFailed();
    }

    realized(true);
}
