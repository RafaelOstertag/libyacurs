// $Id$

#include <cassert>
#include <cstdlib>

#include "dialog.h"

#include "eventqueue.h"

//
// Private
//
Dialog::Dialog(const Dialog&) {
    abort();
}

Dialog&
Dialog::operator=(const Dialog&) {
    abort();
    return *this;
}

//
// Protected
//
void
Dialog::button_press_handler(Event& _e) {
    if (realization()!=REALIZED) return;

    assert(_e==EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(_e);

    assert(__bok!=0);

    if (evt.data()==__bok) {
	__dstate=DIALOG_OK;
	close();
	return;
    }

    if (evt.data()==__bcancel) {
	__dstate=DIALOG_CANCEL;
	close();
	return;
    }
}

//
// Public
//

Dialog::Dialog(const std::string& _title,
	       DIALOG_TYPE _dt,
	       DIALOG_SIZE _ds): Window(),
				 __vpack(0),
				 __hpack(0),
				 __bok(0),
				 __bcancel(0),
				 __dstate(DIALOG_CANCEL),
				 __dialog_type(_dt),
				 __dialog_size(_ds),
				 __title(_title) {
    __vpack=new VPack;
    __hpack=new HPack;

    __vpack->add_back(__hpack);

    switch (__dialog_type) {
    case OKCANCEL:
	__bcancel=new Button("Cancel");
	__hpack->add_back(__bcancel);
    case OK_ONLY:
	__bok=new Button("OK");
	__hpack->add_front(__bok);
	break;
    case YESNO:
	__bcancel=new Button("No");
	__hpack->add_back(__bcancel);
    case YES_ONLY:
	__bok=new Button("Yes");
	__hpack->add_front(__bok);
	break;
    default:
	abort();
	break;
    }

    // from Window
    Window::widget(__vpack);
    frame(true);
}

Dialog::~Dialog() {
    assert(__vpack!=0);
    assert(__hpack!=0);
    assert(__bok!=0);

    delete __vpack;
    delete __hpack;
    delete __bok;

    if (__dialog_type) {
     	assert(__bcancel!=0);
     	delete __bcancel;
    }

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));
}

void
Dialog::widget(WidgetBase* _w) {
    assert(__vpack!=0);
    __vpack->add_front(_w);
}

Dialog::STATE
Dialog::dialog_state() const {
    return __dstate;
}

void
Dialog::refresh(bool immediate) {
    if (realization()!=REALIZED &&
	realization()!=REALIZING) return;

    mymvwaddstr(curses_window(), 0, 1, __title.c_str());

    Window::refresh(immediate);
}

void
Dialog::realize() {
    REALIZE_ENTER;

    EventQueue::connect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));

    Margin _margin(2,2,2,2);
    __vpack->hinting(false);

    if (__dialog_size==AUTOMATIC) {
	__vpack->hinting(true);
	// Compute the margin. We try to vertically center the dialog.
	int hinted_rows = Window::widget()->size_hint().rows();
	if (hinted_rows>0 && hinted_rows<area().rows()-2) {
	    int vert_margin=(area().rows()-hinted_rows)/2-1;
	   _margin=Margin(vert_margin, 2, vert_margin, 2);
	}
    }

    margin(_margin);
    Window::realize();

    REALIZE_LEAVE;
}

void
Dialog::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));

    Window::unrealize();

    UNREALIZE_LEAVE;
}
