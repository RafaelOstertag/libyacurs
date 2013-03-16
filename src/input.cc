// $Id$

#include <cassert>
#include <cstdlib>

#include "input.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "cursex.h"
#include "colors.h"

//
// Private
//

Input&
Input::operator=(const Input&) {
    abort();
    return *this;
}

//
// Protected
//
void
Input::key_handler(Event& _e) {
    assert(_e.type()==EVT_KEY);

    if (!focus()) return;

    EventEx<int>& ekey=dynamic_cast<EventEx<int>&>(_e);

    assert(__offset+__curs_pos<=__buffer.length());

    switch (ekey.data()) {
    case KEY_DOWN:
    case KEY_ENTER:
    case KEY_RETURN:
    case KEY_RETURN2:
    case KEY_TAB:
	EventQueue::submit(EVT_FOCUS_NEXT);
	break;
    case KEY_UP:
    case KEY_BTAB:
	EventQueue::submit(EVT_FOCUS_PREVIOUS);
	break;
    case KEY_DL:
    case KEY_CTRL_U:
	if (__read_only) break;
	__buffer.clear();
	__curs_pos=0;
	__offset=0;
	break;
    case KEY_EOL:
    case KEY_CTRL_K:
	if (__read_only) break;
	__buffer=__buffer.erase(__offset+__curs_pos,
				__buffer.length()-(__offset+__curs_pos));
	break;
    case KEY_BKSPC_SOL:
    case KEY_BACKSPACE:
	if (__read_only) break;
	if (__offset==0 && __curs_pos==0) break;

	if (__offset>0) {
	    __offset--;
	} else {
	    if (__curs_pos>0)
		__curs_pos--;
	}
	if (!__buffer.empty())
	    __buffer=__buffer.erase(__offset+__curs_pos,1);
	break;
    case KEY_CTRL_D:
    case KEY_DC: // Delete
	if (__read_only) break;
	if (__offset+__curs_pos>=__buffer.length() ||
	    __buffer.empty()) break;
	__buffer=__buffer.erase(__offset+__curs_pos,1);
	break;
    case KEY_HOME:
    case KEY_CTRL_A:
	__curs_pos=0;
	__offset=0;
	break;
    case KEY_CTRL_E:
	if (__buffer.length()>=static_cast<std::string::size_type>(__size.cols())) {
	    __offset=__buffer.length()-__size.cols()+1;
	    __curs_pos=__size.cols();
	} else {
	    __offset=0;
	    __curs_pos=__buffer.length();
	}
	break;
    case KEY_LEFT:
    case KEY_CTRL_B:
	if (__curs_pos>0) {
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
    case KEY_CTRL_F:
	if (__curs_pos+__offset>=__buffer.length()) break;

	if (__curs_pos+1==static_cast<std::string::size_type>(__size.cols())) {
	    __offset++;
	} else {
	    // we're somewhere in the widget, but not the end, advance
	    // the cursor position
	    __curs_pos++;
	}
	break;
    default: // regular key presses
	if (__read_only) break;

	// do not overrun the max size
	if (__buffer.length()>=__max_size) break;

	// Add the char to the curses window
	//
	// (void) used to silence clang w/ -Wall -pedantic
	(void)mvwaddch(widget_subwin(), 0, __curs_pos, ekey.data());

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
}
void
Input::realize() {
    REALIZE_ENTER;
	
    Widget::realize();

    EventQueue::connect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    REALIZE_LEAVE;
}

void
Input::unrealize() {
    UNREALIZE_ENTER;
    EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    Widget::unrealize();
    UNREALIZE_LEAVE;
}
//
// Public
//

Input::Input(int _length, std::string::size_type _max_size, const std::string& _t):
    Widget(),
    __offset(0),
    __curs_pos(0),
    __max_size(_max_size),
    __length(_length),
    __read_only(false),
    __buffer(_t.length()>__max_size?_t.substr(0,__max_size):_t),
    __size(__length>0 ? Size(1, __length) : Size::zero()) {
    can_focus(true);
}

Input::~Input() {
    EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));
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

    refresh(true);
}

const std::string&
Input::input() const {
    return __buffer;
}

void
Input::clear() {
    __buffer.clear();
    __curs_pos=0;
    __offset=0;
}

void
Input::readonly(bool _s) {
    __read_only=_s;
}

bool
Input::readonly() const {
    return __read_only;
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

void
Input::refresh(bool immediate) {
    if (realization()!=REALIZED) return;
    assert(widget_subwin()!=0);

    // setting background is essential for coloring entire input
    // widget.

    if (focus()) {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_FOCUS);
	YAPET::UI::Colors::set_bg(widget_subwin(), YAPET::UI::INPUTWIDGET_FOCUS);
	curs_set(1);
	if (leaveok(widget_subwin(), FALSE)==ERR)
	    throw CursesException("leaveok");
    } else {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_NOFOCUS);
	YAPET::UI::Colors::set_bg(widget_subwin(), YAPET::UI::INPUTWIDGET_NOFOCUS);
	curs_set(0);
	if (leaveok(widget_subwin(), TRUE)==ERR)
	    throw CursesException("leaveok");
    }

    if (werase(widget_subwin())==ERR)
	throw CursesException("werase");

    assert(__offset<=__buffer.length());
    // if (mymvwaddstr(widget_subwin(), 0, 0,
    // 		    __label.c_str()) == ERR)
    // 	throw AddStrFailed();
    //
    // We ignore the error returned, since the cursor cannot be
    // advanced past the end, and thus the string is
    // truncated. However, the truncation has no effect on the input.
    if (__buffer.length()>0) {
	mymvwaddstr(widget_subwin(),
		    0, 0,
		    __buffer.substr(__offset, __size.cols()-1).c_str());
	// Sanitize the cursor position if necessary, for example due
	// to a shrink of the screen, the cursor position might
	// overshoot the available subwin size.
	if (__curs_pos>=static_cast<std::string::size_type>(__size.cols()) ) __curs_pos=__size.cols()-1;
    } else {
	// since the buffer is empty, make sure the cursor position is
	// set to the biginning.
	assert(__curs_pos==0); 
    }
    
    if (wmove(widget_subwin(), 0, __curs_pos)==ERR)
	 throw CursesException("wmove");

    Widget::refresh(immediate);
}
