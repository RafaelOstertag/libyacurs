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

int alrm(EventBase& _e) {
    assert(_e == EVT_ALARM);
    EventQueue::inject(EventBase(EVT_QUIT));
    return 0;
}

int main() {
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Basic 1");
	Curses::setTitle(title);

	Window* w1 = new Window(Margin<>(1,0,0,0));
	w1->setFrame(true);

	Curses::setWindow(w1);

	EventQueue::connectEvent(EventConnectorFunction1(EVT_ALARM,&alrm));

	alarm(2);
	Curses::run();

	delete title;
	delete w1;

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
