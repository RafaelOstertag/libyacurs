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

int alrm(Event& _e) {
    static int calls = 0;
    assert(_e == EVT_ALARM);

    std::string status_msg("Size: rows=");
    
    Size _scrdim(Curses::inquiryScreenSize());

    char buff[32];
    snprintf(buff,32,"%d",_scrdim.rows());
    status_msg+=buff;

    status_msg+=" cols=";

    snprintf(buff,32,"%d",_scrdim.cols());
    status_msg+=buff;

    Curses::statusline()->pushMsg(status_msg);

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
    winsize wsave;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wsave) == -1) {
	return 1;
    }

    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Resize 1");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,1,0));
	w1->frame(true);

	Curses::mainwindow(w1);

	StatusLine* sl = new StatusLine();
	Curses::statusline(sl);

	EventQueue::connectEvent(EventConnectorFunction1(EVT_ALARM,&alrm));

	alarm(1);
	Curses::run();

	delete title;
	delete w1;
	delete sl;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
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
