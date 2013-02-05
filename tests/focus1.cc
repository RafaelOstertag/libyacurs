// $Id$
//
// Test Pack
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <debug.h>
#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

// Event handler requires access
Input* ifixed;
Input* idyn;
Button* b1;
Button* b2;
Button* b3;

int button_press_handler(Event& _e) {
    assert(_e==EVT_BUTTON_PRESS);

    EventEx<Button*>& ev=dynamic_cast<EventEx<Button*>&>(_e);
    if (ev.data() == b1) {
	Curses::statusline()->push_msg(ifixed->input());
	return 0;
    }

    if (ev.data() == b2) {
	Curses::statusline()->push_msg(idyn->input());
	return 0;
    }

    if (ev.data() == b3) {
	EventQueue::submit(EVT_QUIT);
	return 0;
    }
    return 1;
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	DEBUG::start();
	Curses::init();

	EventQueue::connect_event(EventConnectorFunction1(EVT_BUTTON_PRESS,button_press_handler));


	Curses::title(new LineObject(LineObject::POS_TOP,
				     "Focus 1"));
	Curses::statusline(new StatusLine);

	Curses::mainwindow(new Window(Margin(1,0,1,0)));
	Curses::mainwindow()->frame(true);

	VPack* vpack=new VPack;
	HPack* hpack=new HPack;
	b1=new Button("Button1");
	b2=new Button("Button2");
	b3=new Button("Button3");
	ifixed=new Input(10);
	idyn=new Input;
	Input* ireadonly=new Input;
	ireadonly->input("Read only");
	ireadonly->readonly(true);
	
	vpack->add_back(ifixed);
	vpack->add_back(idyn);
	vpack->add_back(ireadonly);
	vpack->hinting(false);

	hpack->add_back(b1);
	hpack->add_back(b2);
	hpack->add_back(b3);
	hpack->hinting(false);

	vpack->add_back(hpack);

	assert(b1->label()=="Button1");
	assert(b2->label()=="Button2");
	assert(b3->label()=="Button3");

	Curses::mainwindow()->widget(vpack);

	Curses::run();

	delete b1;
	delete b2;
	delete b3;
	delete idyn;
	delete ifixed;
	delete ireadonly;
	delete hpack;
	delete vpack;
	delete Curses::mainwindow();
	delete Curses::title();
	delete Curses::statusline();
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
