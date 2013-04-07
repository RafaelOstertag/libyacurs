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

#ifdef INTERACTIVE
void key_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_KEY);

    YACURS::EventEx<int>& _ek = dynamic_cast<YACURS::EventEx<int>&>(_e);

    switch (_ek.data()) {
    case 'q':
    case 'Q':
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
        break;

    default:
        break;
    }
}

#else // INTERACTIVE

void alrm(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_SIGALRM);

    std::string status_msg("YACURS::Size: rows=");

    YACURS::Size _scrdim(YACURS::Curses::inquiry_screensize());

    char buff[32];
    snprintf(buff,32,"%d",_scrdim.rows());
    status_msg+=buff;

    status_msg+=" cols=";

    snprintf(buff,32,"%d",_scrdim.cols());
    status_msg+=buff;

    YACURS::Curses::statusline()->push_msg(status_msg);

    winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        return;
    }

    ws.ws_row--;
    ws.ws_col-=5;

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
        return;
    }

    if (ws.ws_row<=YACURS::MIN_ROWS ||
	ws.ws_col<=YACURS::MIN_COLS)
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
    else
        alarm(1);
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
        YACURS::Curses::init();

        YACURS::LineObject* title = new YACURS::LineObject(YACURS::LineObject::POS_TOP,
                                           "DynLabel 1");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1,0,1,0));
        w1->frame(true);

        YACURS::StatusLine* sl = new YACURS::StatusLine();
        YACURS::Curses::statusline(sl);

        YACURS::VPack* vpack = new YACURS::VPack;

        YACURS::DynLabel* dlabel1 = new YACURS::DynLabel("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
        vpack->add_back(dlabel1);

        YACURS::HPack* hpack = new YACURS::HPack;
        vpack->add_back(hpack);

        YACURS::DynLabel* dlabel2 = new YACURS::DynLabel("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
        YACURS::DynLabel* dlabel3 = new YACURS::DynLabel("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
        hpack->add_back(dlabel2);
        hpack->add_back(dlabel3);
        hpack->hinting(false);
        hpack->always_dynamic(true);

        w1->widget(vpack);

        YACURS::Curses::mainwindow(w1);

#ifdef INTERACTIVE
        sl->push_msg("Press Q to quit");
        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(YACURS::EVT_KEY,&key_handler));
#else // INTERACTIVE
        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(YACURS::EVT_SIGALRM,&alrm));
        alarm(2);
#endif
        YACURS::Curses::run();

        delete title;

        delete vpack;
        delete hpack;
        delete dlabel1;
        delete dlabel2;
        delete dlabel3;

        delete w1;
        delete sl;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
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
