// $Id$
//
// Test basic functionality of Curses class
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
    std::list<std::string> items;

    for (int i=0; i<24; i++) {
	std::ostringstream n;
	n<<i;
	items.push_back("Long Name ListBox Item Number " + n.str());
    }
    try {
	DEBUG::start();
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "ListBox 1");
	Curses::title(title);

	// NOTE:
	//
	// The order the objects are created (MyWindow, StatusLine) is
	// important here. Because MyWindow calls
	// StatusLine::put_msg() on resize we have to make sure
	// StatusLine is resized first. Since EventQueue calls the
	// last EventConnector connected first, StatusLine has to be
	// created AFTER MyWindow.

	Window* w1 = new Window(Margin(1,0,1,0));
	w1->frame(true);

	ListBox* lb1 = new ListBox;
	lb1->set(items);

	w1->widget(lb1);

	StatusLine* sl = new StatusLine();
	Curses::statusline(sl);
	sl->push_msg("Press Q to quit");

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_KEY,&key_handler));

	Curses::run();

	delete title;
	delete lb1;
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
