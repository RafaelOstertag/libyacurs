// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "event.h"

////////////////////////////////////////////////////////
//
// class Event
//
////////////////////////////////////////////////////////

Event::Event(EVENT_TYPE _et): event_type(_et) {}

Event::Event(const Event& _e) {
    event_type = _e.event_type;
}

Event&
Event::operator=(const Event& _e) {
    event_type = _e.event_type;
    return *this;
}

Event::~Event() {}

bool
Event::operator==(const Event& _e) const {
    return event_type == _e.event_type;
}

bool
Event::operator==(EVENT_TYPE _et) const {
    return event_type == _et;
}

EVENT_TYPE
Event::type() const {
    return event_type;
}

Event*
Event::clone() const {
    return new Event(*this);
}

Event::operator EVENT_TYPE() const {
    return event_type;
}

std::string
Event::evt2str(EVENT_TYPE _et) {
    const char* __str_table[] = {
	"EVT_QUIT",
	"EVT_SIGWINCH",
	"EVT_SIGALRM",
	"EVT_KEY",
	"EVT_REFRESH",
	"EVT_FORCEREFRESH",
	"EVT_DOUPDATE",
	"EVT_TERMRESETUP",
	"EVT_SIGUSR1",
	"EVT_SIGUSR2",
	"EVT_SIGINT",
	"EVT_FOCUS_NEXT",
	"EVT_FOCUS_PREVIOUS",
	"EVT_WINDOW_SHOW",
	"EVT_WINDOW_CLOSE",
	"EVT_BUTTON_PRESS",
	"EVT_LISTBOX_ENTER",
	NULL
    };

    return __str_table[_et];
}
