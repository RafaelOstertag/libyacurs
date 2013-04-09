// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gettext.h>

#include <cassert>
#include <cstdlib>

#include "dialog.h"
#include "eventqueue.h"
#include "yacursex.h"

#ifdef ENABLE_NLS
# define _(String) dgettext(PACKAGE, String)
#else
# define _(String) (String)
#endif

using namespace YACURS;

//
// Private
//
Dialog::Dialog(const Dialog&) {
    throw NotSupported();
}

Dialog&
Dialog::operator=(const Dialog&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
const Button* const
Dialog::ok_button() const {
    return __bok;
}

const Button* const
Dialog::cancel_button() const {
    return __bcancel;
}

void
Dialog::dialog_state(STATE st) {
    __dstate=st;
}

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
	       DIALOG_SIZE _ds): __vpack(0),
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
	__bcancel=new Button(_("Cancel"));
	__hpack->add_back(__bcancel);
    case OK_ONLY:
	__bok=new Button(_("OK"));
	__hpack->add_front(__bok);
	break;
    case YESNO:
	__bcancel=new Button(_("No"));
	__hpack->add_back(__bcancel);
    case YES_ONLY:
	__bok=new Button(_("Yes"));
	__hpack->add_front(__bok);
	break;
    default:
	throw InvalidDialogType();
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

    YACURS::INTERNAL::CurStr tmp(__title,Coordinates(1,0));
    curses_window()->addstrx(tmp);

    Window::refresh(immediate);
}

void
Dialog::realize() {
    REALIZE_ENTER;

    // Reset the dialog state
    __dstate=DIALOG_CANCEL;

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
