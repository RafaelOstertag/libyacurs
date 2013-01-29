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

class MyWindow: public Window {
    protected:
	int resize_handler(Event& _e) {
	    Window::resize_handler(_e);

	    EventWinCh& winch = dynamic_cast<EventWinCh&>(_e);
	    std::string status_msg("Size: rows=");
	    
	    char buff[32];
	    snprintf(buff,32,"%d",winch.data().rows());
	    status_msg+=buff;
	    
	    status_msg+=" cols=";
	    
	    snprintf(buff,32,"%d",winch.data().cols());
	    status_msg+=buff;
	    
	    Curses::statusline()->push_msg(status_msg);
	    return 0;
	};

    public:
	MyWindow() : Window() {
	    EventQueue::connect_event(EventConnectorMethod1<MyWindow>(EVT_SIGWINCH,this, &MyWindow::resize_handler));
	}
	MyWindow(const Margin& _m) : Window(_m) {}
	MyWindow(const MyWindow& _o) : Window(_o) {}

};

int alrm(Event& _e) {
    assert(_e == EVT_SIGALRM);
    EventQueue::submit(Event(EVT_QUIT));
    return 0;
}


int main() {
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Resize 3");
	Curses::title(title);

	MyWindow* w1 = new MyWindow(Margin(1,0,1,0));
	w1->frame(true);

	Curses::mainwindow(w1);

	StatusLine* sl = new StatusLine();
	Curses::statusline(sl);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

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
