// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>

#include "checkbox.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursex.h"
#include "colors.h"

using namespace YACURS;

class MaxStrLen {
    private:
	std::string::size_type __max_len;

    public:
	MaxStrLen(): __max_len(0) {}

	std::string::size_type max_len() const {
	    return __max_len;
	}

	void operator()(const std::string& _s) {
	    __max_len=std::max(_s.length(), __max_len);
	}
};

//
// Private
//

CheckBox&
CheckBox::operator=(const CheckBox&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
void
CheckBox::key_handler(Event& _e) {
    assert(_e.type()==EVT_KEY);

    if (!focus()) return;

    EventEx<int>& ekey=dynamic_cast<EventEx<int>&>(_e);

    switch (ekey.data()) {
    case KEY_TAB:
	EventQueue::submit(EVT_FOCUS_NEXT);
	break;
    case KEY_ENTER:
    case KEY_RETURN:
    case KEY_RETURN2:
    case ' ':
	set_selection(__cursor);
	break;
    case KEY_DOWN:
	if (__cursor < __items.size()-1)
	    __cursor++;
	else
	    __cursor=0;
	break;
    case KEY_UP:
	if (__cursor > 0)
	    __cursor--;
	else
	    __cursor=__items.size()-1;
	break;
    case KEY_BTAB:
	EventQueue::submit(EVT_FOCUS_PREVIOUS);
	break;
    }

    refresh(true);
}

void
CheckBox::set_selection(unsigned short _cursor) {
    assert(_cursor<__items.size());
    __items[_cursor].selected=!__items[_cursor].selected;
}

//
// Public
//

CheckBox::CheckBox(const std::string& _title,
		   const std::vector<std::string>& _items):
    __size(Size::zero()),
    __cursor(0),
    __title(_title) {
    __indicators[0] = "[ ] ";
    __indicators[1] = "[x] ";
    can_focus(true);
    
    MaxStrLen len = std::for_each(_items.begin(), _items.end(), MaxStrLen());

    __items.resize(_items.size());
    std::copy(_items.begin(), _items.end(), __items.begin());

    // +2 because we have a border. +6 because we add the check box
    // indicators and the border.
    __size=Size(__items.size()+2, len.max_len()+6);
}

CheckBox::~CheckBox() {
    EventQueue::disconnect_event(EventConnectorMethod1<CheckBox>(EVT_KEY,this, &CheckBox::key_handler));
}

bool
CheckBox::selected(unsigned short _i) {
    if (__items.size()<_i) {
	std::ostringstream _ind;
	_ind << _i;
	throw std::out_of_range(_("CheckBox: index ") + _ind.str() + _(" out of range."));
    }

    return __items[_i].selected;
}

bool
CheckBox::selected(const std::string& _i) {
    for(std::vector<Selectable>::size_type n=0;
	n<__items.size(); n++)
	if (__items[n].item == _i)
	    return __items[n].selected;

    throw std::out_of_range(_("Item '") + _i + _("' not found in CheckBox"));
}

void
CheckBox::size_available(const Size& _s) {
    WidgetBase::size_available(_s);
}

Size
CheckBox::size() const {
    return __size;
}

Size
CheckBox::size_hint() const {
    return __size;
}

bool
CheckBox::size_change() {
    return false;
}

void
CheckBox::reset_size() {
    // Intentionaly empty
}

void
CheckBox::refresh(bool immediate) {
    if (realization()!=REALIZED) return;
    assert(widget_subwin()!=0);

    std::vector<Selectable>::iterator it = __items.begin();
    std::vector<Selectable>::size_type pos=0;
    std::string item;
    while (++pos,it!=__items.end()) {
	item=__indicators[(*it).selected?1:0] + (*it).item;
	widget_subwin()->addstr(YACURS::INTERNAL::CurStr(item,Coordinates(1,pos)));
	it++;
    }

    if (focus()) {
	widget_subwin()->box(0,0);
    } else {
	widget_subwin()->box('|', '-');
    }

    if (!__title.empty()) {
	widget_subwin()->addstrx(YACURS::INTERNAL::CurStr(__title,Coordinates(1,0)));
    }

    if (focus()) {
	curs_set(1);
	widget_subwin()->leaveok(false);
	widget_subwin()->move(Coordinates(2,__cursor+1));
    } else {
	curs_set(0);
	widget_subwin()->leaveok(true);
    }

    Widget::refresh(immediate);
}

void
CheckBox::realize() {
    REALIZE_ENTER;
	
    Widget::realize();

    EventQueue::connect_event(EventConnectorMethod1<CheckBox>(EVT_KEY,this, &CheckBox::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    REALIZE_LEAVE;
}

void
CheckBox::unrealize() {
    UNREALIZE_ENTER;
    EventQueue::disconnect_event(EventConnectorMethod1<CheckBox>(EVT_KEY,this, &CheckBox::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    Widget::unrealize();
    UNREALIZE_LEAVE;
}
