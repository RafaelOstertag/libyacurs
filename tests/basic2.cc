// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "yacurs.h"

int alrm(Event& _e) {
    static int calls = 0;
    assert(_e == EVT_ALARM);

    switch (calls) {
    case 0:
	Curses::getStatusLine()->pushMsg("Status 1");
	break;
    case 1:
	Curses::getStatusLine()->pushMsg("Status 2");
	break;
    case 2:
	Curses::getStatusLine()->popMsg();
	break;
    case 3:
	Curses::getStatusLine()->popMsg();
	break;
    }

    if (calls++ > 3)
	EventQueue::submit(Event(EVT_QUIT));
    else
	alarm(1);

    return 0;
}

int main() {
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Basic 2");
	Curses::setTitle(title);

	Window* w1 = new Window(Margin(1,0,1,0));
	w1->frame(true);

	Curses::setWindow(w1);

	StatusLine* sl = new StatusLine();
	Curses::setStatusLine(sl);

	EventQueue::connectEvent(EventConnectorFunction1(EVT_ALARM,&alrm));

	alarm(1);
	Curses::run();

	delete title;
	delete w1;
	delete sl;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
