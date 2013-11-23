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

#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==0) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Pack 2");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::HPack* hpack = new YACURS::HPack;

        YACURS::VPack* vpack1 = new YACURS::VPack;
	YACURS::VPack* vpack2 = new YACURS::VPack;

        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

#ifdef YACURS_USE_WCHAR
        YACURS::Label* label1 = new YACURS::Label("¡Test label¿");
#else
        YACURS::Label* label1 = new YACURS::Label("Test label");
#endif
        vpack1->add_front(label1);

#ifdef YACURS_USE_WCHAR
        YACURS::Label* label2 = new YACURS::Label("Test label²");
#else
        YACURS::Label* label2 = new YACURS::Label("Test label2");
#endif
        vpack1->add_back(label2);

#ifdef YACURS_USE_WCHAR
        YACURS::Label* label3 = new YACURS::Label("Test label³");
        YACURS::Label* label4 = new YACURS::Label("Test label⁴");
#else
        YACURS::Label* label3 = new YACURS::Label("Test label3");
        YACURS::Label* label4 = new YACURS::Label("Test label4");
#endif
        vpack2->add_back(label3);
        vpack2->add_back(label4);

        w1->widget(hpack);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(3);
        YACURS::Curses::run();

        delete title;
        delete label1;
        delete label2;
        delete label3;
        delete label4;
        delete vpack1;
        delete vpack2;
        delete hpack;
        delete w1;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
