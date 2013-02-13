// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

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

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "yacurs.h"
#include "debug.h"

class MyWindow: public Window {
    protected:
	void resize_handler(Event& _e) {
	    Window::resize_handler(_e);

	    EventEx<Size>& winch = dynamic_cast<EventEx<Size>&>(_e);
	    std::string status_msg("Size: rows=");
	    
	    char buff[32];
	    snprintf(buff,32,"%d",winch.data().rows());
	    status_msg+=buff;
	    
	    status_msg+=" cols=";
	    
	    snprintf(buff,32,"%d",winch.data().cols());
	    status_msg+=buff;
	    
	    Curses::statusline()->push_msg(status_msg);
	};

    public:
	MyWindow() : Window() {
	    EventQueue::connect_event(EventConnectorMethod1<MyWindow>(EVT_SIGWINCH,this, &MyWindow::resize_handler));
	}
	MyWindow(const Margin& _m) : Window(_m) {}
	MyWindow(const MyWindow& _o) : Window(_o) {}

};

void key_handler(Event& _e) {
    assert(_e == EVT_KEY);
 
    EventEx<int>& _ek = dynamic_cast<EventEx<int>&>(_e);

    switch (_ek.data()) {
    case 'q':
    case 'Q':
	EventQueue::submit(Event(EVT_QUIT));
	break;
    default:
	break;
    }
}

int main() {
    try {
	DEBUG::start();
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Resize 3");
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
	Curses::statusline(sl);
	sl->push_msg("Press Q to quit");

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_KEY,&key_handler));

	Curses::run();

	delete title;
	delete w1;
	delete sl;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
