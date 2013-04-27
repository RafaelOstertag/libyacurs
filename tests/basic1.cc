// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "yacurs.h"

YACURS::TitleBar* title = 0;

void
alrm(YACURS::Event& _e) {
    static int counter = 0;

    assert(_e == YACURS::EVT_SIGALRM);
    assert(title != 0);

    switch (counter) {
    case 0:
        counter++;
        title->alignment(YACURS::TitleBar::RIGHT);
        alarm(1);
        break;

    case 1:
        counter++;
        title->alignment(YACURS::TitleBar::CENTER);
        alarm(1);
        break;

    case 2:
        counter++;
        title->alignment(YACURS::TitleBar::LEFT);
        alarm(1);
        break;

    default:
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
        break;
    }
}

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        YACURS::Curses::init();

        title = new YACURS::TitleBar(YACURS::TitleBar::POS_TOP,
                                     "Basic 1");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(1);
        YACURS::Curses::run();

        delete title;
        delete w1;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
