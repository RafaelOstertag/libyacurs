// $Id$
//
// Test Pack
#ifdef HAVE_CONFIG_H
#include "config.h"
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

#ifdef USE_WCHAR
    setlocale(LC_ALL, "");
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Pack 1");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::VPack* vpack = new YACURS::VPack;
        w1->widget(vpack);

        YACURS::Label* label1 = new YACURS::Label("Test label");
        vpack->add_front(label1);

        YACURS::Label* label2 = new YACURS::Label("Test label2");
        vpack->add_back(label2);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(3);
        YACURS::Curses::run();

        delete title;
        delete label1;
        delete label2;
        delete vpack;
        delete w1;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
