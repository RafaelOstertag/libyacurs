// $Id: resize4.cc 4802 2013-02-03 14:42:46Z rafisol $
//
// Test resizing with overlapping windows.
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_TERMIOS_H
# include <termios.h>
#else // HAVE_TERMIOS_H
# ifdef HAVE_SYS_TERMIOS_H
#  include <sys/termios.h>
# endif // HAVE_SYS_TERMIOS_H
#endif // HAVE_TERMIOS_H

#include <iostream>
#include <sstream>

#include "yacurs.h"
#include "debug.h"

class MyWindow: public Window {
    private:
	Window* win;
    public:
	void show() {
	    Window::show();
	    win=new Window(Margin(3,3,3,3));
	    win->frame(true);
	    win->show();
	}

	void close() {
	    assert(win!=NULL);
	    win->close();
	    delete win;
	    win=NULL;
	}
	    
	MyWindow(const Margin& m) : Window(m), win(NULL) {
	}
	MyWindow(const MyWindow& _o) : Window(_o) {}

};

int key_handler(Event& _e) {
    assert(_e == EVT_KEY);
 
    EventKey& _ek = dynamic_cast<EventKey&>(_e);

    switch (_ek.data()) {
    case 'q':
    case 'Q':
	EventQueue::submit(Event(EVT_QUIT));
	break;
    default:
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
	DEBUG::start();
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Resize 5: Overlapping windows");
	Curses::title(title);

	// NOTE:
	// 
	// The order the objects are created (MyWindow, StatusLine) is
	// important here. Because MyWindow calls
	// StatusLine::put_msg() on resize we have to make sure
	// StatusLine is resized first. Since EventQueue calls the
	// last EventConnector connected first, StatusLine has to be
	// created AFTER MyWindow.

	MyWindow* w1 = new MyWindow(Margin(1,0,1,0));
	w1->frame(true);

	StatusLine* sl = new StatusLine();
	sl->push_msg("Press Q to quit");
	Curses::statusline(sl);

	HPack* hpack = new HPack;
	Label* hl1 = new Label("abcdefghijklmnopqrstuvwxyz");
	Label* hl2 = new Label("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	Label* hl3 = new Label("0123456789");

    
	hpack->add_front(hl1);
	hpack->add_front(hl2);
	hpack->add_back(hl3);

	VPack* vpack = new VPack;
	Label* vls[20];
	for(int i=0; i<20; i++) {
	    std::ostringstream _i;
	    _i<<i;
	    vls[i]=new Label("VLabel " + _i.str());
	    vpack->add_back(vls[i]);
	}
	
	hpack->add_front(vpack);

	w1->widget(hpack);

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_KEY,&key_handler));

	Curses::run();

	delete vpack;
	for(int i=0; i<20; i++) {
	    delete vls[i];
	}
	delete title;
	delete hl1;
	delete hl2;
	delete hl3;
	delete hpack;
	delete w1;
	delete sl;
	Curses::end();
	DEBUG::end();
    } catch (std::exception& e) {
	Curses::end();
	DEBUG::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}