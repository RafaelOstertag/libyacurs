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

int alrm(Event& _e) {
    assert(_e == EVT_SIGALRM);
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
					   "Pack 2");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);


	HPack* hpack = new HPack;

	VPack* vpack1 = new VPack;
	VPack* vpack2 = new VPack;

	hpack->add_back(vpack1);
	hpack->add_back(vpack2);
	

	Label* label1 = new Label("Test label");
	vpack1->add_front(label1);
	Label* label2 = new Label("Test label2");
	vpack1->add_back(label2);

	Label* label3 = new Label("Test label3");
	Label* label4 = new Label("Test label4");
	vpack2->add_back(label3);
	vpack2->add_back(label4);

	w1->widget(hpack);

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

	alarm(1);
	Curses::run();

	delete title;
	delete label1;
	delete label2;
	delete label3;
	delete label4;
	delete vpack1;
	delete vpack2;
	delete hpack;
	delete w1;

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
