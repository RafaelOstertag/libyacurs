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

void
alrm(YACURS::Event& _e) {
    static int calls = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    switch (calls) {
    case 0:
        YACURS::Curses::statusline()->push_msg("Status 1");
        break;

    case 1:
        YACURS::Curses::statusline()->push_msg("Status 2");
        break;

    case 2:
        YACURS::Curses::statusline()->pop_msg();
        break;

    case 3:
        YACURS::Curses::statusline()->pop_msg();
        break;
    }

    if (calls++ > 3)
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
    else
        alarm(1);
}

int
main() {
    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Basic 2");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);

        YACURS::Curses::mainwindow(w1);

        YACURS::StatusLine* sl = new YACURS::StatusLine();
        YACURS::Curses::statusline(sl);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(1);
        YACURS::Curses::run();

        delete title;
        delete w1;
        delete sl;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
