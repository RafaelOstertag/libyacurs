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

#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif // HAVE_SYS_TERMIOS_H

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "yacurs.h"

int main() {
    try {
	Curses::init();

	Window* w1 = new Window(Margin<>(1,0,1,0));
	w1->setFrame(true);

	w1->realize();
	w1->refresh(true);

	sleep(1);

	Rectangle<> scrsz(Curses::inquiryScreenSize());
	Rectangle<> newsz(0,0,scrsz.rows()-1, scrsz.cols()-1);
	w1->resize(newsz);
	w1->refresh(true);

	sleep(1);

	newsz=Rectangle<>(0,0,scrsz.rows()-2, scrsz.cols()-2);
	w1->resize(newsz);
	w1->refresh(true);

	sleep(1);

	delete w1;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
