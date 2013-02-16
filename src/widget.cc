// $Id$

#include <cassert>
#include <cstdlib>


#include "cursex.h"
#include "widget.h"
#include "eventqueue.h"

//
// Private
//

Widget&
Widget::operator=(const Widget&) {
    abort();
    return *this;
}

//
// Protected
//
void
Widget::force_refresh_handler(Event& _e) {
    if (realization()!=REALIZED) return;

    assert(_e == EVT_FORCEREFRESH);
    assert(__widget_subwin!=NULL);

    if (clearok(__widget_subwin, TRUE)==ERR)
	throw ClearOKFailed();
}

void
Widget::unrealize() {
    UNREALIZE_ENTER;
    WidgetBase::unrealize();

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));
    
    assert(__widget_subwin!=NULL);

    // We have to clear the window since the new size might be
    // smaller, and thus leaving artifacts on the screen if we omit to
    // clear the entire subwin()
    if (wclear(__widget_subwin) == ERR) {
	realization(UNREALIZED);
	throw ClearFailed();
    }

    if (delwin(__widget_subwin) == ERR) {
	realization(UNREALIZED);
	throw DelWindowFailed();
    }

    __widget_subwin = NULL;

    // This is also needed to remove artifacts on the screen
    if (touchwin(curses_window()) == ERR) {
	realization(UNREALIZED);
	throw TouchFailed();
    }
    if (wrefresh(curses_window()) == ERR) {
	realization(UNREALIZED);
	throw RefreshFailed();
    }

    UNREALIZE_LEAVE;
}

WINDOW*
Widget::widget_subwin() const {
    return __widget_subwin;
}

//
// Public
//

Widget::Widget(): WidgetBase(), __widget_subwin(NULL) {
}

Widget::~Widget() {
    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,this, &Widget::force_refresh_handler));
    
    if (realization()==REALIZED) {
	assert(__widget_subwin!=NULL);
	if (delwin(__widget_subwin)==ERR)
	    throw DelWindowFailed();
    }
}
    
void
Widget::refresh(bool immediate) {
    if (!(realization()==REALIZED ||
	  realization()==REALIZING) ) return;

    assert(__widget_subwin!=NULL);
    assert(focusgroup_id()!=(fgid_t)-1);

    int retval;
    if (immediate)
	retval = wrefresh(__widget_subwin);
    else
	retval = wnoutrefresh(__widget_subwin);

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
    if (!realization()==REALIZED) return;

    unrealize();

    position(_a);
    size_available(_a);

    realize();
    

}

void
Widget::realize() {
    REALIZE_ENTER;

    const Coordinates& pos = position();
    const Size& _size = size();
    const Size& size_a = size_available();

    assert(focusgroup_id()!=(fgid_t)-1);

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
    assert(__widget_subwin==NULL);

    __widget_subwin = ::subwin(curses_window(),
		       _size.rows(),
		       _size.cols(),
		       pos.y(),
		       pos.x());
    if (__widget_subwin == NULL) {
	realization(UNREALIZED);
	throw SubwinFailed();
    }

    if (scrollok(__widget_subwin, FALSE)==ERR) {
	realization(UNREALIZED);
	delwin(__widget_subwin);
	__widget_subwin=NULL;
	throw ScrollOKFailed();
    }

    if (leaveok(__widget_subwin, TRUE)==ERR) {
	realization(UNREALIZED);
	delwin(__widget_subwin);
	__widget_subwin=NULL;
	throw LeaveOKFailed();
    }

    REALIZE_LEAVE;
}
