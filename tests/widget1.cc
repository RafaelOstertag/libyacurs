// $Id$
//
// Add a Label to a Window, show, and quit.
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ENABLE_NLS
#include <locale.h>
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

void
alrm(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_SIGALRM);
    YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
}

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Widget 1");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::Label* label1 = new YACURS::Label("Test label");
        w1->widget(label1);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(1);
        YACURS::Curses::run();

        delete title;
        delete label1;
        delete w1;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
