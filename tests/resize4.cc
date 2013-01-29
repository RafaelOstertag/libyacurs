// $Id: resize2.cc 4738 2013-01-25 16:55:19Z rafisol $
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

#include "yacurs.h"
#include "debug.h"

int alrm(Event& _e) {
    static int calls = 0;
    assert(_e == EVT_SIGALRM);

    std::string status_msg("Size: rows=");
    
    Size _scrdim(Curses::inquiry_screensize());

    char buff[32];
    snprintf(buff,32,"%d",_scrdim.rows());
    status_msg+=buff;

    status_msg+=" cols=";

    snprintf(buff,32,"%d",_scrdim.cols());
    status_msg+=buff;

    Curses::statusline()->push_msg(status_msg);

    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
	return -1;
    }

    ws.ws_row--;
    ws.ws_col--;

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
	return -1;
    }

    if (calls++ > 3)
	EventQueue::submit(Event(EVT_QUIT));
    else
	alarm(1);

    return 0;
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    winsize wsave;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wsave) == -1) {
	return 1;
    }

    try {
	DEBUG::start();
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Resize 4");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,1,0));
	w1->frame(true);

	HPack* hpack = new HPack;
	Label* l1 = new Label("abcdefghijklmnopqrstuvwxyz");
	Label* l2 = new Label("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	Label* l3 = new Label("0123456789");

	hpack->add_front(l1);
	hpack->add_front(l2);
	hpack->add_back(l3);

	w1->widget(hpack);

	Curses::mainwindow(w1);

	StatusLine* sl = new StatusLine();
	Curses::statusline(sl);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

	alarm(2);
	Curses::run();

	delete title;
	delete l1;
	delete l2;
	delete l3;
	delete hpack;
	delete w1;
	delete sl;
	Curses::end();
	DEBUG::end();
    } catch (std::exception& e) {
	Curses::end();
	DEBUG::end();
	if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
	    return 1;
	}
	std::cerr << e.what() << std::endl;
	return 1;
    }

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
	return 1;
    }

    return 0;
}
