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

// The alarm handler needs access to those
HPack* hpack;
VPack* vpack1;
VPack* vpack2;

Label* label1;
Label* label2;
Input* input1;
Input* input2;

int alrm(Event& _e) {
    static int i=0;

    assert(_e == EVT_SIGALRM);
    switch (i++) {
    case 0:
	input1->input("abcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyz");
	break;
    case 1:
	input2->input("abcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyz");
	break;
    case 2:
	label1->label("abcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyz");
	break;
    case 3:
	label2->label("abcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyzabcdefghiklmnopqrstuvwxyz");
	break;
    default:
	EventQueue::submit(Event(EVT_QUIT));
	break;
    }

    alarm(1);
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
					   "Widget 2: setting text in realized Input/Label");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);


	hpack = new HPack;
	vpack1 = new VPack;
	vpack2 = new VPack;

	label1 = new Label("Label1");
	input1 = new Input;
	vpack1->add_back(label1);
	vpack1->add_back(input1);
	vpack1->hinting(false);

	label2 = new Label("Label2");
	input2 = new Input;
	vpack2->add_back(label2);
	vpack2->add_back(input2);
	vpack2->hinting(false);

	hpack->add_back(vpack1);
	hpack->add_back(vpack2);

	w1->widget(hpack);

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

	alarm(5);
	Curses::run();

	delete title;
	delete hpack;
	delete vpack1;
	delete vpack2;
	delete label1;
	delete label2;
	delete input1;
	delete input2;
	delete w1;

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
