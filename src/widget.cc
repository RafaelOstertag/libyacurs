// $Id$

#include <cassert>

#include "debug.h"

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
    if (realization()!=REALIZED) return 0;

    assert(_e == EVT_FORCEREFRESH);
    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin!=NULL);

    if (clearok(*__widget_subwin, TRUE)==ERR)
	throw ClearOKFailed();

    return 0;
}

void
Widget::unrealize() {
    UNREALIZE_ENTER;

    DEBUGOUT("-- IN: Widget::unrealize()");
    DEBUGOUT(*this);

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));
    
    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin!=NULL);

    // We have to clear the window since the new size might be
    // smaller, and thus leaving artifacts on the screen if we omit to
    // clear the entire subwin()
    if (wclear(*__widget_subwin) == ERR) {
	realization(UNREALIZED);
	throw ClearFailed();
    }

    if (delwin(*__widget_subwin) == ERR) {
	realization(UNREALIZED);
	throw DelWindowFailed();
    }

    *__widget_subwin = NULL;

    // This is also needed to remove artifacts on the screen
    if (touchwin(curses_window()) == ERR) {
	realization(UNREALIZED);
	throw TouchFailed();
    }
    if (wrefresh(curses_window()) == ERR) {
	realization(UNREALIZED);
	throw RefreshFailed();
    }

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: Widget::unrealize()");

    UNREALIZE_LEAVE;
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
}

Widget::Widget(const Widget& _w):
    WidgetBase(_w), __instance_count(_w.__instance_count),
    __widget_subwin(_w.__widget_subwin) {
    (*__instance_count)++;
}

Widget::~Widget() {
    assert(__instance_count!=NULL);
    if (*__instance_count > 1) {
	(*__instance_count)--;
	return;
    }

    delete __instance_count;

    assert(__widget_subwin!=NULL);
    
    bool delwin_failed=false;;
    if (realization()==REALIZED)
	delwin_failed=delwin(*__widget_subwin)==ERR;

    delete __widget_subwin;

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));

    if (delwin_failed)
	throw DelWindowFailed();
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
    if (!(realization()==REALIZED ||
	  realization()==REALIZING) ) return;

    DEBUGOUT("-- IN: Widget::refresh()");
    DEBUGOUT(*this);

    assert(__widget_subwin!=NULL);
    assert(*__widget_subwin!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*__widget_subwin);
    else
	retval = wnoutrefresh(*__widget_subwin);

    if (retval == ERR)
	throw RefreshFailed();

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: Widget::refresh()");
}
	
void
Widget::resize(const Area& _a) {
    //
    // 1. Keep in mind: a resize does not refresh!
    //
    // 2. The actual resize has to be done in a derived class
    //
    if (!realization()==REALIZED) return;
    DEBUGOUT("-- IN: Widget::resize()");
    DEBUGOUT(*this);

    unrealize();

    position(_a);
    size_available(_a);

    realize();
    
    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: Widget::resize()");
}

void
Widget::realize() {
    REALIZE_ENTER;

    DEBUGOUT("-- IN: Widget::realize()");
    DEBUGOUT(*this);

    const Coordinates& pos = position();
    const Size& _size = size();
    const Size& size_a = size_available();

    EventQueue::connect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));

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
	realization(UNREALIZED);
	throw SubwinFailed();
    }

    if (scrollok(*__widget_subwin, FALSE)==ERR) {
	realization(UNREALIZED);
	delwin(*__widget_subwin);
	*__widget_subwin=NULL;
	throw ScrollOKFailed();
    }

    if (leaveok(*__widget_subwin, TRUE)==ERR) {
	realization(UNREALIZED);
	delwin(*__widget_subwin);
	*__widget_subwin=NULL;
	throw LeaveOKFailed();
    }

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: Widget::realize()");

    REALIZE_LEAVE;
}

Widget::operator std::string() const {
    return "Widget{\n\t(pos:" + 
	static_cast<std::string>(position()) + ")\n\t(" +
	"szav:" + static_cast<std::string>(size_available()) + ")\n\t(" +
	"sz:" + static_cast<std::string>(size()) + ")\n\t(" +
	"hint:" + static_cast<std::string>(size_hint()) + ")\n}";
}
