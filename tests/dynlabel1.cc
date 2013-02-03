// $Id: packresize1.cc 4738 2013-01-25 16:55:19Z rafisol $
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

#ifdef INTERACTIVE
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

#else // INTERACTIVE

int alrm(Event& _e) {
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
    ws.ws_col-=5;

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
	return -1;
    }

    if (ws.ws_row<=MIN_ROWS ||
	ws.ws_col<=MIN_COLS)
	EventQueue::submit(Event(EVT_QUIT));
    else
	alarm(1);

    return 0;
}
#endif // INTERACTIVE

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifndef INTERACTIVE
    winsize wsave;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wsave) == -1) {
	return 1;
    }
#endif // INTERACTIVE
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "DynLabel 1");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,1,0));
	w1->frame(true);

	StatusLine* sl = new StatusLine();
	Curses::statusline(sl);

	VPack* vpack = new VPack;

	DynLabel* dlabel1 = new DynLabel("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	vpack->add_back(dlabel1);

	HPack* hpack = new HPack;
	vpack->add_back(hpack);

	DynLabel* dlabel2 = new DynLabel("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	DynLabel* dlabel3 = new DynLabel("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	hpack->add_back(dlabel2);
	hpack->add_back(dlabel3);
	hpack->hinting(false);
	hpack->always_dynamic(true);

	w1->widget(vpack);

	Curses::mainwindow(w1);

#ifdef INTERACTIVE
	sl->push_msg("Press Q to quit");
	EventQueue::connect_event(EventConnectorFunction1(EVT_KEY,&key_handler));
#else // INTERACTIVE
	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));
	alarm(2);
#endif
	Curses::run();

	delete title;

	delete vpack;
	delete hpack;
	delete dlabel1;
	delete dlabel2;
	delete dlabel3;

	delete w1;
	delete sl;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
#ifndef INTERACTIVE
	if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
	    return 1;
	}
#endif // INTERACTIVE
	std::cerr << e.what() << std::endl;
	return 1;
    }

#ifndef INTERACTIVE
    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
	return 1;
    }
#endif // INTERACTIVE

    return 0;
}
