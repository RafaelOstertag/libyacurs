// $Id: widget3.cc 4710 2013-01-20 18:27:44Z rafisol $
//
// Test Label size change while displaying.
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

// The alarm handler needs access to those
Label* label1;
Label* label3;

int alrm(Event& _e) {
    static int i=0;

    assert(_e == EVT_ALARM);
    switch (i++) {
    case 0:
	label1->label("Big New Label1");
	alarm(1);
	break;
	
    case 1:
	label1->label("Small label1");
	alarm(1);
	break;

    case 2:
	label1->label("This should resize");
	alarm(1);
	break;

    case 3:
	label3->label("Huge Big Label 3");
	alarm(1);
	break;
	
    default:
	EventQueue::submit(Event(EVT_QUIT));
	break;
    }
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
					   "Widget 4");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);


	HPack* hpack = new HPack;

	VPack* vpack1 = new VPack;
	VPack* vpack2 = new VPack;

	hpack->add_back(vpack1);
	hpack->add_back(vpack2);
	

	label1 = new Label("Test label");
	vpack1->add_front(label1);
	Label* label2 = new Label("Test label2");
	vpack1->add_back(label2);

	label3 = new Label("Test label3");
	Label* label4 = new Label("Test label4");
	vpack2->add_back(label3);
	vpack2->add_back(label4);

	label1->label("New Test Label");
	label2->label("New Test Label2");
	label3->label("New Test Label3");
	label4->label("New Test Label4");

	w1->widget(hpack);

	Curses::mainwindow(w1);

	EventQueue::connectEvent(EventConnectorFunction1(EVT_ALARM,&alrm));

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
