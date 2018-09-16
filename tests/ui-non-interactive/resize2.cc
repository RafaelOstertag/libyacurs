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

void alrm(YACURS::Event& _e) {
    static int calls = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    std::string status_msg("YACURS::Size: rows=");

    YACURS::Size _scrdim(YACURS::Curses::inquiry_screensize());

    char buff[32];
    snprintf(buff, 32, "%d", _scrdim.rows());
    status_msg += buff;

    status_msg += " cols=";

    snprintf(buff, 32, "%d", _scrdim.cols());
    status_msg += buff;

    YACURS::Curses::statusbar()->push(status_msg);

    winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        return;
    }

    ws.ws_row--;
    ws.ws_col--;

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
        return;
    }

    if (calls++ > 3)
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
    else
        alarm(1);
}

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL, "en_US.UTF-8") == 0) exit(77);
#endif

    winsize wsave;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wsave) == -1) {
        return 1;
    }

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title =
            new YACURS::TitleBar(YACURS::TitleBar::POS_TOP, "Resize 2");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0));
        w1->frame(true);

        YACURS::Curses::mainwindow(w1);

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        YACURS::Curses::statusbar(sl);

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorFunction1(YACURS::EVT_SIGALRM, &alrm));

        alarm(1);
        YACURS::Curses::run();

        delete title;
        delete w1;
        delete sl;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();

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
