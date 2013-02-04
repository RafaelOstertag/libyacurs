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

// buttons require access to them
Input* ifixed;
Input* idyn;

class Button1: public Button {
    protected:
	void on_press() {
	    Curses::statusline()->push_msg(ifixed->input());
	}
    public:
	Button1(): Button("Button1") {}
};

class Button2: public Button {
    protected:
	void on_press() {
	    Curses::statusline()->push_msg(idyn->input());
	}
    public:
	Button2(): Button("Button2") {}
};

class Button3: public Button {
    protected:	
	void on_press() {
	    EventQueue::submit(EVT_QUIT);
	}
    public:
	Button3(): Button("Button3") {}
};

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	DEBUG::start();
	Curses::init();


	Curses::title(new LineObject(LineObject::POS_TOP,
				     "Focus 1"));
	Curses::statusline(new StatusLine);


	Curses::mainwindow(new Window(Margin(1,0,1,0)));
	Curses::mainwindow()->frame(true);

	VPack* vpack=new VPack;
	HPack* hpack=new HPack;
	Button1* b1=new Button1;
	Button2* b2=new Button2;
	Button3* b3=new Button3;
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
