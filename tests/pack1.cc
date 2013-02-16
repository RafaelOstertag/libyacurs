// $Id$
//
// Test Pack
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

void alrm(Event& _e) {
    assert(_e == EVT_SIGALRM);
    EventQueue::submit(Event(EVT_QUIT));
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Pack 1");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);

	VPack* vpack = new VPack;
	w1->widget(vpack);

	Label* label1 = new Label("Test label");
	vpack->add_front(label1);

	Label* label2 = new Label("Test label2");
	vpack->add_back(label2);

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

	alarm(1);
	Curses::run();

	delete title;
	delete label1;
	delete label2;
	delete vpack;
	delete w1;

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
