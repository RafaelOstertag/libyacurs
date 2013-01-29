// $Id$

#include <cassert>

#include "cursex.h"
#include "widget.h"
#include "eventqueue.h"

//
// Private
//

//
// Protected
//
int
Widget::force_refresh_handler(Event& _e) {
    if (!realized()) return 0;

    assert(_e == EVT_FORCEREFRESH);
    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin!=NULL);

    if (clearok(*__widget_subwin, TRUE)==ERR)
	throw ClearOKFailed();

    return 0;
}

void
Widget::unrealize() {
    if (not realized()) return;

    realized(false);

    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin!=NULL);

    // We have to clear the window since the new size might be
    // smaller, and thus leaving artifacts on the screen if we omit to
    // clear the entire subwin()
    if (wclear(*__widget_subwin) == ERR)
	throw ClearFailed();

    if (delwin(*__widget_subwin) == ERR)
	throw DelWindowFailed();
    *__widget_subwin = NULL;

    // This is also needed to remove artifacts on the screen
    if (touchwin(curses_window()) == ERR)
	throw TouchFailed();
    if (wrefresh(curses_window()) == ERR)
	throw RefreshFailed();


}

WINDOW*
Widget::widget_subwin() const {
    return *__widget_subwin;
}

//
// Public
//

Widget::Widget():
    WidgetBase(), __instance_count(NULL), __widget_subwin(NULL) {
    __widget_subwin = new WINDOW*;
    *__widget_subwin = NULL;

    __instance_count = new unsigned int;
    *__instance_count = 1;

    EventQueue::connect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));
}

Widget::Widget(const Widget& _w):
    WidgetBase(_w), __instance_count(_w.__instance_count),
    __widget_subwin(_w.__widget_subwin) {
    (*__instance_count)++;
    EventQueue::connect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));
}

Widget::~Widget() {
    assert(__instance_count!=NULL);
    if (*__instance_count > 1) {
	(*__instance_count)--;
	return;
    }

    delete __instance_count;

    assert(__widget_subwin!=NULL);

    if (*__widget_subwin != NULL)
	if (delwin(*__widget_subwin) == ERR)
	    throw DelWindowFailed();

    delete __widget_subwin;

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));
}

Widget&
Widget::operator=(const Widget& _w) {
    if (this == &_w)
	return *this;

    WidgetBase::operator=(_w);

    __instance_count = _w.__instance_count;
    assert(__instance_count!=NULL);
    (*__instance_count)++;

    __widget_subwin = _w.__widget_subwin;

    return *this;
}
    
void
Widget::refresh(bool immediate) {
    if (!realized()) return;

    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*__widget_subwin);
    else
	retval = wnoutrefresh(*__widget_subwin);

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
    if (!realized()) return;

    unrealize();

    position(_a);
    size_available(_a);

    realize();
}

void
Widget::realize() {
    if (realized()) return;

    const Coordinates& pos = position();
    const Size& _size = size();
    const Size& size_a = size_available();

    // We cannot assert on parent() since it might be legally NULL
    // assert(parent()!=NULL

    assert(_size!=Size::zero());
    assert(size_a!=Size::zero());

    if (size_a.rows()<1 ||
	_size.rows()>size_a.rows()||
	_size.cols()>size_a.cols() )
	return;

    assert(curses_window()!=NULL);
    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin==NULL);

    *__widget_subwin = ::subwin(curses_window(),
		       _size.rows(),
		       _size.cols(),
		       pos.y(),
		       pos.x());
    if (*__widget_subwin == NULL) {
	throw SubwinFailed();
    }

    if (scrollok(*__widget_subwin, FALSE)==ERR)
	throw ScrollOKFailed();

    if (leaveok(*__widget_subwin, TRUE)==ERR)
	throw LeaveOKFailed();

    realized(true);
}
