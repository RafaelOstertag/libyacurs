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

class Button1: public Button {
    protected:
	void focus(bool _f) {
	    Button::focus(_f);
	    if (_f) 
		Curses::statusline()->push_msg("Button 1 focused");
	    else
		Curses::statusline()->push_msg("Button 1 unfocused");
	}
	void on_press() {
	    Curses::statusline()->push_msg("Button 1 pressed");
	}
    public:
	Button1(): Button("[ Button1 ]") {}
};

class Button2: public Button {
    protected:
	void focus(bool _f) {
	    Button::focus(_f);
	    if (_f) 
		Curses::statusline()->push_msg("Button 2 focused");
	    else
		Curses::statusline()->push_msg("Button 2 unfocused");
	}
	void on_press() {
	    Curses::statusline()->push_msg("Button 2 pressed");
	}
    public:
	Button2(): Button("[ Button2 ]") {}
};

class Button3: public Button {
    protected:
	void focus(bool _f) {
	    Button::focus(_f);
	    if (_f) 
		Curses::statusline()->push_msg("Button 3 focused");
	    else
		Curses::statusline()->push_msg("Button 3 unfocused");
	}
	void on_press() {
	    EventQueue::submit(EVT_QUIT);
	}
    public:
	Button3(): Button("[ Button3 ]") {}
};

int main() {
#if 1
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	Curses::init();

	
	Curses::title(new LineObject(LineObject::POS_TOP,
				     "Pack 9:"));
	Curses::statusline(new StatusLine);


	Curses::mainwindow(new Window(Margin(1,0,1,0)));
	Curses::mainwindow()->frame(true);

	HPack* hpack=new HPack;
	Button1* b1=new Button1;
	Button2* b2=new Button2;
	Button3* b3=new Button3;

	hpack->add_back(b1);
	hpack->add_back(b2);
	hpack->add_back(b3);

	Curses::mainwindow()->widget(hpack);

	Curses::run();

	delete b1;
	delete b2;
	delete b3;
	delete hpack;
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
