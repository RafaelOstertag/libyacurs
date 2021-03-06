// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif  // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif  // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif  // HAVE_STROPTS_H

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#else  // HAVE_TERMIOS_H
#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif  // HAVE_SYS_TERMIOS_H
#endif  // HAVE_TERMIOS_H

#include <iostream>

#include "yacurs.h"

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL, "en_US.UTF-8") == 0) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0));
        w1->frame(true);

        w1->realize();
        w1->refresh(true);

#ifdef SLOW_TESTS
        sleep(1);
#endif

        YACURS::Size scrsz(YACURS::Curses::inquiry_screensize());
        YACURS::Area newsz(0, 0, scrsz.rows() - 1, scrsz.cols() - 1);
        w1->resize(newsz);
        w1->refresh(true);

#ifdef SLOW_TESTS
        sleep(1);
#endif

        newsz = YACURS::Area(0, 0, scrsz.rows() - 2, scrsz.cols() - 2);
        w1->resize(newsz);
        w1->refresh(true);

#ifdef SLOW_TESTS
        sleep(1);
#endif

        delete w1;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
