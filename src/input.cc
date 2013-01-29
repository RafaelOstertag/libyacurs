// $Id$

#include <cassert>

#include "input.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "cursex.h"
#include "colors.h"

//
// Private
//
void
Input::visibly_change_focus() {
    if (__focus) {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_FOCUS);

	if (leaveok(widget_subwin(), FALSE)==ERR)
	    throw LeaveOKFailed();
    } else {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_NOFOCUS);
	if (leaveok(widget_subwin(), TRUE)==ERR)
	    throw LeaveOKFailed();
    }

    // we have to immediately refresh to let the colors take effect.
    refresh(true);
}

//
// Protected
//
int
Input::key_handler(Event& _e) {
    assert(_e.type()==EVT_KEY);

    if (!__focus) return 0;

    EventKey& ekey=dynamic_cast<EventKey&>(_e);

    switch (ekey.data()) {
    case KEY_DOWN:
    case KEY_ENTER:
    case '\r':
    case '\t':
	EventQueue::submit(EVT_FOCUS_NEXT);
	break;
    case KEY_UP:
    case KEY_BTAB:
	EventQueue::submit(EVT_FOCUS_PREVIOUS);
	break;
    case 21: // Ctrl-U
	__buffer.clear();
	__curs_pos=0;
	__offset=0;
	break;
    case 11: // Ctrl-K
	#warning "to be done"
	break;
    case KEY_LEFT:
	if (__curs_pos>1) {
	    __curs_pos--;
	} else {
	    if (__offset>0) {
		__offset--;
	    } else {
		if (__curs_pos>0)
		    __curs_pos--;
	    }
	}
	break;
    case KEY_RIGHT:
	if (__curs_pos+__offset>=__buffer.length()) break;

	if (__curs_pos+1==static_cast<std::string::size_type>(__size.cols())) {
	    __offset++;
	} else {
	    // we're somewhere in the widget, but not the end, advance
	    // the cursor position
	    __curs_pos++;
	}
	break;
    case KEY_BACKSPACE:
	// Check if the cursor is at the very beginning, of so, bail
	// out.
	if (__curs_pos==0 &&
	    __offset==0) break;

	if (__curs_pos>1) {
	    __curs_pos--;
	} else {
	    // we start `paging' horizontally, so that most of the
	    // __buffer is visible
	    if (__offset>0) {
		if (__offset>=static_cast<std::string::size_type>(__size.cols())) {
		    __offset-=__size.cols()-1;
		    __curs_pos=__size.cols()-1;
		} else {
		    __curs_pos+=__offset;
		    __offset=0;
		}
	    } else {
		if (__curs_pos>0)
		    __curs_pos--;
	    }
	}
	if (!__buffer.empty())
	    __buffer=__buffer.erase(__offset+__curs_pos,1);
	break;
	
    default: // regular key presses
	// do not overrun the max size
	if (__buffer.length()>=__max_size) break;

	// Add the char to the curses window
	mvwaddch(widget_subwin(), 0, __curs_pos, ekey.data());

	// Add (insert) the char to the buffer. No cursor motion, this
	// is done in the next block.
	if (__offset+__curs_pos>__buffer.length()) {
	    // we would exceed the buffer, so we push it back at the end
	    assert(__offset+__curs_pos==__buffer.length()+1);
	    __buffer.push_back(ekey.data());
	} else {
	    // we're somewhere in the middle of the buffer, insert the
	    // char there.
	    __buffer.insert(__offset+__curs_pos, 1, ekey.data());
	}

	// Make sure the __curs_pos does not overshoot the
	// border of the window
	assert(__curs_pos<static_cast<std::string::size_type>(__size.cols()));

	// Advance the cursor position. If __curs_pos+1 hits
	// the border, advance the offset. This way we always have a
	// space at the end of the string (on the screen only, not in
	// the __buffer of course).
	if (__curs_pos+1==static_cast<std::string::size_type>(__size.cols())) {
	    __offset++;
	} else {
	    // we're somewhere in the widget, but not the end, advance
	    // the cursor position
	    __curs_pos++;
	}
	break;
    }

    refresh(true);

    return 0;
}


void
Input::realize() {
    Widget::realize();

    FocusManager::current_focus_group_add(this);
}

void
Input::unrealize() {
    FocusManager::current_focus_group_remove(this);

    Widget::unrealize();
}
//
// Public
//

Input::Input(int _length, std::string::size_type _max_size, const std::string& _t):
    Widget(),
    __focus(false),
    __offset(0),
    __curs_pos(0),
    __max_size(_max_size),
    __length(_length),
    __buffer(_t.length()>__max_size?_t.substr(0,__max_size):_t),
    __size() {
    if (__length>0)
	__size=Size(1,__length);

    EventQueue::connect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));
}

Input::Input(const Input& _i): Widget(_i),
			       __focus(_i.__focus),
			       __offset(_i.__offset),
			       __curs_pos(_i.__curs_pos),
			       __max_size(_i.__max_size),
			       __length(_i.__length),
			       __buffer(_i.__buffer),
			       __size(_i.__size) {
    EventQueue::connect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));
}

Input::~Input() {
    EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));}

Input&
Input::operator=(const Input& _i) {
    Widget::operator=(_i);
    __focus=_i.__focus;
    __offset=_i.__offset;
    __curs_pos=_i.__curs_pos;
    __max_size=_i.__max_size;
    __length=_i.__length;
    __buffer=_i.__buffer;
    __size=_i.__size;
    return *this;
}

void
Input::input(const std::string& i) {
    if (i.length()>__max_size)
	__buffer=i.substr(0,__max_size);
    else
	__buffer=i;

    // Reset cursor position
    __offset=0;
    __curs_pos=0;

    if (realized())
	refresh(true);
}

std::string
Input::input() const {
    return __buffer;
}

void
Input::size_available(const Size& _s) {
    WidgetBase::size_available(_s);

    // if we're dynamic, take the cols from _s as cols for our
    // __size. Else, do nothing.
    if (__length==0) {
	__size.rows(1); // this is fixed for Input Widgets
	__size.cols(_s.cols());
    } 
    assert(__size.rows()<=_s.rows());
    assert(__size.cols()<=_s.cols());
}

Size
Input::size() const {
    return __size;
}

Size
Input::size_hint() const {
    // if __length != 0 then we're not dynmically sized, and the ctor
    // has set the size, which we return. See also comment for
    // size_hint() in widgetbase.h.
    if (__length!=0) return __size;

    return Size(1,__buffer.length());
}

bool
Input::size_change() {
    return false;
}

void
Input::reset_size() {
    // If __length != 0 then we are not dynamically sized, thus not
    // resetting size.
    if (__length!=0) return;

    __size=Size::zero();
}

bool
Input::can_focus() const {
    return true;
}

void
Input::focus(bool _f) {
    __focus = _f;

    if (realized())
	visibly_change_focus();
}

bool
Input::focus() const {
    return __focus;
}

void
Input::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    assert(widget_subwin()!=NULL);

    if (__focus) {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_FOCUS);
    } else {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_NOFOCUS);
    }

    if (werase(widget_subwin())==ERR)
	throw EraseFailed();

    assert(__offset<=__buffer.length());
    // if (mymvwaddstr(widget_subwin(), 0, 0,
    // 		    __label.c_str()) == ERR)
    // 	throw AddStrFailed();
    //
    // We ignore the error returned, since the cursor cannot be
    // advanced past the end, and thus the string is
    // truncated. However, the truncation has no effect on label.
    if (__buffer.length()>0) {
	mymvwaddstr(widget_subwin(),
		    0, 0,
		    __buffer.substr(__offset,
				    std::min(__buffer.length(),
					     static_cast<std::string::size_type>(__size.cols()-1)) ).c_str());
    }

    // Ignore wmove() errors for now
    //
    if (wmove(widget_subwin(), 0, __curs_pos)==ERR)
	 throw WMoveFailed();
    //wmove(widget_subwin(), 0, __curs_pos);

    wcursyncup(widget_subwin());

    Widget::refresh(immediate);
}
