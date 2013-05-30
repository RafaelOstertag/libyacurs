// $Id$
//
// Test Label size change while displaying.
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

// The alarm handler needs access to those
YACURS::Label* label1;
YACURS::Label* label3;

void
alrm(YACURS::Event& _e) {
    static int i = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    switch (i++) {
    case 0:
        label1->label("Big New Label1");
        alarm(2);
        break;

    case 1:
        label1->label("Small label1");
        alarm(2);
        break;

    case 2:
        label1->label("This should resize");
        alarm(2);
        break;

    case 3:
        label3->label("Huge Big Label 3");
        alarm(2);
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

#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Pack 3: Widget resizes");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::HPack* hpack = new YACURS::HPack;

        YACURS::VPack* vpack1 = new YACURS::VPack;
        YACURS::VPack* vpack2 = new YACURS::VPack;

        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

        label1 = new YACURS::Label("Test label");
        vpack1->add_front(label1);
        YACURS::Label* label2 = new YACURS::Label("Test label2");
        vpack1->add_back(label2);

        label3 = new YACURS::Label("Test label3");
        YACURS::Label* label4 = new YACURS::Label("Test label4");
        vpack2->add_back(label3);
        vpack2->add_back(label4);

        label1->label("New Test Label");
        label2->label("New Test Label2");
        label3->label("New Test Label3");
        label4->label("New Test Label4");

        w1->widget(hpack);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(2);
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
