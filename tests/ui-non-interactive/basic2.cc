// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

void
alrm(YACURS::Event& _e) {
    static int calls = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    switch (calls) {
    case 0:
#ifdef USE_WCHAR
	YACURS::Curses::statusbar()->push_msg("Status ¹");
#else
        YACURS::Curses::statusbar()->push_msg("Status 1");
#endif
        break;

    case 1:
#ifdef USE_WCHAR
	YACURS::Curses::statusbar()->push_msg("Status ²");
#else
        YACURS::Curses::statusbar()->push_msg("Status 2");
#endif
        break;

    case 2:
        YACURS::Curses::statusbar()->pop_msg();
        break;

    case 3:
        YACURS::Curses::statusbar()->pop_msg();
        break;
    }

    if (calls++ > 3)
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
    else
        alarm(1);
}

int
main() {
#ifdef USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==NULL) exit(77);
#endif
    try {
        YACURS::Curses::init();

#ifdef USE_WCHAR
        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Basic ²");
#else
        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Basic 2");
#endif
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);

        YACURS::Curses::mainwindow(w1);

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        YACURS::Curses::statusbar(sl);

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