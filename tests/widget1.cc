// $Id: basic1.cc 4677 2013-01-14 20:05:04Z rafisol $
//
// Test widget
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

int alrm(Event& _e) {
    assert(_e == EVT_ALARM);
    EventQueue::submit(Event(EVT_QUIT));
    return 0;
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Basic 1");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);

	Label* label1 = new Label("Test label");
	w1->widget(label1);

	Curses::mainwindow(w1);

	EventQueue::connectEvent(EventConnectorFunction1(EVT_ALARM,&alrm));

	alarm(1);
	Curses::run();

	delete title;
	delete label1;
	delete w1;

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}