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
    if (__focus)
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_FOCUS);
    else
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_NOFOCUS);

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
    }

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
    __max_size(_max_size),
    __length(_length),
    __buffer(_t),
    __size() {
    if (__length>0)
	__size=Size(1,__length);

    EventQueue::connect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));
}

Input::Input(const Input& _i): Widget(_i),
			       __focus(_i.__focus),
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
    __max_size=_i.__max_size;
    __length=_i.__length;
    __buffer=_i.__buffer;
    __size=_i.__size;
    return *this;
}

void
Input::input(const std::string& i) {
    __buffer=i;

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

    // if we're dynamical, take the cols from _s as cols for our
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
    // if __length != 0 then we're not dynmically size, and the ctor
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
#warning "Not fully implemented"
    if (!realized()) throw NotRealized();

    assert(widget_subwin()!=NULL);

    if (__focus)
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_FOCUS);
    else
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::INPUTWIDGET_NOFOCUS);

    // if (mymvwaddstr(widget_subwin(), 0, 0,
    // 		    __label.c_str()) == ERR)
    // 	throw AddStrFailed();
    //
    // We ignore the error returned, since the cursor cannot be
    // advanced past the end, and thus the string is
    // truncated. However, the truncation has no effect on label.
    mymvwaddstr(widget_subwin(), 0, 0,  __buffer.c_str());

    Widget::refresh(immediate);
}
