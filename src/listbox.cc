// $Id$

#include <cassert>
#include <sstream>

#include "listbox.h"
#include "debug.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "cursex.h"
#include "colors.h"

//
// Private
//
void
ListBox::visibly_change_focus() {
    assert(0);
}

//
// Protected
//
int
ListBox::key_handler(Event& _e) {
    assert(_e==EVT_KEY);

    if (!__focus) return 0;

    EventKey& ekey=dynamic_cast<EventKey&>(_e);

    switch (ekey.data()) {
    case KEY_ENTER:
    case KEY_RETURN:
    case KEY_RETURN2:
	EventQueue::submit(EventEx<ListBox*>(EVT_LISTBOX_ENTER,this));
	break;
    case KEY_TAB:
	EventQueue::submit(EVT_FOCUS_NEXT);
	break;
    case KEY_BTAB:
	EventQueue::submit(EVT_FOCUS_PREVIOUS);
	break;
    case KEY_DOWN:
    case KEY_CTRL_N: // Emacs key
    case 'j': // VIM key
	if ((__offset+__curs_pos)>__list.size() ||
	    __list.empty()) break;

	if (__curs_pos<__size.rows()-3 && /* We have to take the box into account */
	    __curs_pos<__list.size()-1) {
	    __curs_pos++;
	} else {
	    if (__offset+__curs_pos+1 < __list.size())
		__offset++;
	}

	break;
    case KEY_UP:
    case KEY_CTRL_P: // Emacs key
    case 'k': // VIM key
	if ((__offset==0 && __curs_pos==0) ||
	    __list.empty()) break;

	if (__curs_pos>0)
	    __curs_pos--;
	else
	    __offset--;

	break;
	// Solaris' X/Open Curses and System Curses do not support
	// HOME key on PC keyboard, i.e. they tell me it has code `27'
	// which is ESC.
    case KEY_HOME:
    case KEY_CTRL_A:
	__offset=__curs_pos=0;
	break;
	// Solaris' X/Open Curses and System Curses do not support
	// END key on PC keyboard, i.e. they tell me it has code `27'
	// which is ESC.
    case KEY_END:
    case KEY_CTRL_E:
	if (__list.empty()) break;

	if (__list.size()<=__size.rows()-2) {
	    __curs_pos=__list.size()-1;
	} else {
	    __offset=__list.size()-__size.rows()+2;
	    __curs_pos=__size.rows()-3;
	}
	break;
    case KEY_NPAGE:
	if (__list.size()<=__size.rows()-2 ||
	    __list.empty()) break;

	if (__offset+__size.rows()-2<=__list.size()-__size.rows()+2)
	    __offset+=__size.rows()-2;
	else
	    __offset=__list.size()-__size.rows()+2;

	break;
    case KEY_PPAGE:
	if (__list.size()<=__size.rows()-2 ||
	    __list.empty()) break;

	if (__offset>__size.rows()-2)
	    __offset-=__size.rows()-2;
	else
	    __offset=0;
	break;
    }

    refresh(true);

    return 0;
}

void
ListBox::realize() {
    REALIZE_ENTER;

    DEBUGOUT("-- IN: ListBox<T>::realize()");
    DEBUGOUT(*this);

    Widget::realize();

    EventQueue::connect_event(EventConnectorMethod1<ListBox>(EVT_KEY,this, &ListBox::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    FocusManager::focus_group_add(focusgroup_id(), this);

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: ListBox<T>::realize()");

    REALIZE_LEAVE;
}

void
ListBox::unrealize() {
    UNREALIZE_ENTER;

    DEBUGOUT("-- IN: ListBox<T>::unrealize()");
    DEBUGOUT(*this);

    EventQueue::disconnect_event(EventConnectorMethod1<ListBox>(EVT_KEY,this, &ListBox::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    FocusManager::focus_group_remove(focusgroup_id(), this);

    Widget::unrealize();
    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: ListBox<T>::unrealize()");

    UNREALIZE_LEAVE;
}

//
// Public
//

ListBox::ListBox(): Widget(),
		 __list(),
		 __focus(false),
		 __offset(0),
		 __curs_pos(0),
		 __size(Size::zero()) {}

ListBox::ListBox(const ListBox& _lb): Widget(_lb),
				      __list(_lb.__list),
				      __focus(_lb.__focus),
				      __offset(_lb.__offset),
				      __curs_pos(_lb.__curs_pos),
				      __size(_lb.__size) {}
ListBox::~ListBox() {
    EventQueue::disconnect_event(EventConnectorMethod1<ListBox>(EVT_KEY, this, &ListBox::key_handler));
}

ListBox&
ListBox::operator=(const ListBox& _i) {
    Widget::operator=(_i);

    __list = _i.__list;
    __focus = _i.__focus;
    __offset = _i.__offset;
    __curs_pos = _i.__curs_pos;
    __size = _i.__size;

    return *this;
}

void
ListBox::add(const std::string& _i) {
    __list.push_back(_i);
    __list.sort();

    if (realization()==REALIZED)
	refresh(true);
}

void
ListBox::set(const std::list<std::string>& _l) {
    __list=_l;
    __list.sort();

    // Reset these. I don't see a proper way of maintaining these when
    // a completely new `data set' is set.
    __offset=0;
    __curs_pos=0;

    if (realization()==REALIZED)
	refresh(true);
}

std::list<std::string>::size_type
ListBox::selected() const {
    return __curs_pos + __offset;
}

void
ListBox::size_available(const Size& _s) {
    WidgetBase::size_available(__size=_s);
}

Size
ListBox::size() const {
    return __size;
}

Size
ListBox::size_hint() const {
    return Size::zero();
}

bool
ListBox::size_change() {
    return false;
}

void
ListBox::reset_size() {
    __size=Size::zero();
}

bool
ListBox::can_focus() const {
    return true;
}

void
ListBox::focus(bool _f) {
    __focus=_f;

    if (realization()==REALIZED)
	refresh(true);
}

bool
ListBox::focus() const {
    return __focus;
}

void
ListBox::refresh(bool immediate) {
    if (realization()!=REALIZED) return;

    DEBUGOUT("-- IN: ListBox<T>::refresh()");
    DEBUGOUT(*this);

    assert(widget_subwin()!=NULL);

    std::list<std::string>::iterator it=__list.begin();

    // Make sure cursor position is not off the list, i.e. on the
    // border of the widget.
    __curs_pos=__curs_pos>__size.rows()-3 ? __size.rows()-3 : __curs_pos;

    // Make sure the offset will not produce an out of bound, for
    // instance due to a screen resize.
    if (__size.rows()-2>__list.size()||
	__size.rows()-2+__offset>__list.size())
	__offset=0; // we must not use an offset.

    // Advance to offset
    for(std::list<std::string>::size_type i=0;
	i<__offset;
	it++, i++);

    for(std::list<std::string>::size_type i=0;
	i<std::min<std::list<std::string>::size_type>(__size.rows()-2, __list.size());
	it++, i++) {

#warning "If kept, add error checking"
	if (__focus && i==__curs_pos)
	    wattrset(widget_subwin(), A_REVERSE);
	else
	    wattrset(widget_subwin(), A_NORMAL);

	// We prepare our own line, so that we can fill up with spaces
	std::string line(*it);

	// Find out if we have to indicate that the line has been truncated
	if (line.length()<=__size.cols()-2) {
	    // line is shorter than visible area
	    if (line.length()<__size.cols()-2)
		line.append((__size.cols()-2)-line.length(),' ');
	    mymvwaddnstr(widget_subwin(),
			 i+1,1,
			 line.c_str(), __size.cols()-2);
	} else {
	    mymvwaddnstr(widget_subwin(),
			 i+1,1,
			 line.c_str(), __size.cols()-3);
	    winsch(widget_subwin(), '>');
	}

#warning "If kept, add error checking"
	wattrset(widget_subwin(), A_NORMAL);
    }

    //
    // Box creation and scroll marker setting was moved here, because
    // it didn't work properly, i.e. there were artifcats on the right
    // side of the box, when it is placed in front of the above code
    // block.
    //
    if (box(widget_subwin(), 0, 0)==ERR)
	throw BoxFailed();

    // set scroll markers
    if (__list.size()>__size.rows()-2) {
	// Can we scroll up? This is indicated by an __offset > 0
	if (__offset>0)
	    mvwaddch(widget_subwin(), 1, __size.cols()-1, '^');
	// can we scroll further down?
	if (__offset+__size.rows()-3<__list.size()-1)
	    mvwaddch(widget_subwin(), __size.rows()-2, __size.cols()-1, 'v');
    }

    // Set the cursor at the right position if we have focus.
    if (__focus && !__list.empty()) {
	curs_set(1);
	if (leaveok(widget_subwin(), FALSE)==ERR)
	    throw LeaveOKFailed();

	if (wmove(widget_subwin(), __curs_pos+1, 1)==ERR)
	    throw WMoveFailed();
    } else {
	curs_set(0);
	if (leaveok(widget_subwin(), TRUE)==ERR)
	    throw LeaveOKFailed();
    }

    Widget::refresh(immediate);

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: ListBox<T>::refresh()");
}

ListBox::operator std::string() const  {
    std::ostringstream _f, _o, _p, _s;
    _f << __focus;
    _o << __offset;
    _p << __curs_pos;
    _s << __list.size();
    return "ListBox<T>{\n\t(focus:" +
	_f.str() + ")\n\t(" +
	"offset:" + _o.str() + ")\n\t(" +
	"curspos:" + _p.str()  + ")\n\t(" +
	"size:" + _s.str() + ")\n}";
}
