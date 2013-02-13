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

LineObject* title=NULL;

void alrm(Event& _e) {
    static int counter=0;

    assert(_e == EVT_SIGALRM);
    assert(title != NULL);

    switch (counter) {
    case 0:
	counter++;
	title->alignment(LineObject::RIGHT);
	alarm(1);
	break;
    case 1:
	counter++;
	title->alignment(LineObject::CENTER);
	alarm(1);
	break;
    case 2:
	counter++;
	title->alignment(LineObject::LEFT);
	alarm(1);
	break;
    default:
	EventQueue::submit(Event(EVT_QUIT));
	break;
    }
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	Curses::init();

	title = new LineObject(LineObject::POS_TOP,
					   "Basic 1");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

	alarm(1);
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
