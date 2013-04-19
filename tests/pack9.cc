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

// The alarm handler needs access to those
YACURS::VPack* vpack;
YACURS::HPack* hpack;
YACURS::HPack* hpack1;
YACURS::HPack* hpack2;
YACURS::VPack* vpack1;
YACURS::VPack* vpack2;
YACURS::VPack* vpack2_1;
YACURS::VPack* vpack2_2;

YACURS::Label* label1;
YACURS::Label* label2;
YACURS::Label* label3;
YACURS::Label* label4;
YACURS::Label* label5;
YACURS::Label* label6;
YACURS::Label* label7;
YACURS::Label* label8;
YACURS::Label* label9;
YACURS::Label* label10;
YACURS::Label* label11;
YACURS::Label* label12;
YACURS::Label* label13;
YACURS::Label* label14;
YACURS::Label* label15;
YACURS::Label* label16;

void
alrm(YACURS::Event& _e) {
    static int i = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    switch (i++) {
    case 0:
        YACURS::Curses::title()->line(
            "Pack 9: always_dynamic=false, hinting=false");
        hpack->always_dynamic(false);
        hpack->hinting(false);
        hpack1->always_dynamic(false);
        hpack1->hinting(false);
        hpack2->always_dynamic(false);
        hpack2->hinting(false);
        vpack1->always_dynamic(false);
        vpack1->hinting(false);
        vpack2->always_dynamic(false);
        vpack2->hinting(false);
        vpack2_1->always_dynamic(false);
        vpack2_1->hinting(false);
        vpack2_2->always_dynamic(false);
        vpack2_2->hinting(false);
        alarm(2);
        break;

    case 1:
        YACURS::Curses::title()->line(
            "Pack 9: always_dynamic=true, hinting=false");
        hpack->always_dynamic(true);
        hpack->hinting(false);
        hpack1->always_dynamic(true);
        hpack1->hinting(false);
        hpack2->always_dynamic(true);
        hpack2->hinting(false);
        vpack1->always_dynamic(true);
        vpack1->hinting(false);
        vpack2->always_dynamic(true);
        vpack2->hinting(false);
        vpack2_1->always_dynamic(true);
        vpack2_1->hinting(false);
        vpack2_2->always_dynamic(true);
        vpack2_2->hinting(false);
        alarm(2);
        break;

    case 2:
        YACURS::Curses::title()->line(
            "Pack 9: always_dynamic=false, hinting=true");
        hpack->always_dynamic(false);
        hpack->hinting(true);
        hpack1->always_dynamic(false);
        hpack1->hinting(true);
        hpack2->always_dynamic(false);
        hpack2->hinting(true);
        vpack1->always_dynamic(false);
        vpack1->hinting(true);
        vpack2->always_dynamic(false);
        vpack2->hinting(true);
        vpack2_1->always_dynamic(false);
        vpack2_1->hinting(true);
        vpack2_2->always_dynamic(false);
        vpack2_2->hinting(true);
        alarm(2);
        break;

    case 3:
        YACURS::Curses::title()->line(
            "Pack 9: always_dynamic=true, hinting=true");
        hpack->always_dynamic(true);
        hpack->hinting(true);
        hpack1->always_dynamic(true);
        hpack1->hinting(true);
        hpack2->always_dynamic(true);
        hpack2->hinting(true);
        vpack1->always_dynamic(true);
        vpack1->hinting(true);
        vpack2->always_dynamic(true);
        vpack2->hinting(true);
        vpack2_1->always_dynamic(true);
        vpack2_1->hinting(true);
        vpack2_2->always_dynamic(true);
        vpack2_2->hinting(true);
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

    try {
        YACURS::Curses::init();

        YACURS::LineObject* title = new YACURS::LineObject(
            YACURS::LineObject::POS_TOP,
            "Pack 9:");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        vpack = new YACURS::VPack;
        hpack = new YACURS::HPack;
        hpack1 = new YACURS::HPack();
        hpack2 = new YACURS::HPack();
        vpack1 = new YACURS::VPack;
        vpack2 = new YACURS::VPack;
        vpack2_1 = new YACURS::VPack();
        vpack2_2 = new YACURS::VPack();

        label1 = new YACURS::Label("Test label");
        label2 = new YACURS::Label("Test label2");
        label3 = new YACURS::Label("Test label3");
        label4 = new YACURS::Label("Test label4");
        label5 = new YACURS::Label("Label 5");
        label6 = new YACURS::Label("Label 6");
        label7 = new YACURS::Label("Label 7");
        label8 = new YACURS::Label("Label 8");
        label9 = new YACURS::Label("Label 9");
        label10 = new YACURS::Label("Label 10");
        label11 = new YACURS::Label("Label 11");
        label12 = new YACURS::Label("Label 12");
        label13 = new YACURS::Label("Label 13");
        label14 = new YACURS::Label("Label 14");
        label15 = new YACURS::Label("Label 15");
        label16 = new YACURS::Label("Label 16");

        vpack->add_front(hpack);
        vpack->add_front(hpack1);
        vpack->add_back(hpack2);

        hpack->always_dynamic(true);
        hpack->hinting(true);
        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

        hpack1->always_dynamic(true);
        hpack1->hinting(true);
        hpack1->add_back(vpack2_1);
        hpack1->add_back(vpack2_2);

        vpack1->always_dynamic(true);
        vpack1->hinting(false);
        vpack1->add_back(label1);
        vpack1->add_back(label2);
        vpack1->add_front(label3);

        vpack2->always_dynamic(true);
        vpack2->hinting(false);
        vpack2->add_back(label4);
        vpack2->add_back(label5);
        vpack2->add_back(label6);

        vpack2_1->always_dynamic(true);
        vpack2_1->hinting(false);
        vpack2_1->add_front(label7);
        vpack2_1->add_front(label8);
        vpack2_1->add_front(label9);

        vpack2_2->always_dynamic(true);
        vpack2_2->hinting(false);
        vpack2_2->add_back(label10);
        vpack2_2->add_back(label11);
        vpack2_2->add_back(label12);
        vpack2_2->add_back(label13);
        vpack2_2->add_back(label14);

        hpack2->always_dynamic(true);
        hpack2->hinting(true);
        hpack2->add_back(label15);
        hpack2->add_back(label16);

        label1->label("New Test Label");
        label2->label("New Test Label2");
        label3->label("New Test Label3");
        label4->label("New Test Label4");

        w1->widget(vpack);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(5);
        YACURS::Curses::run();

        delete title;
        delete label1;
        delete label2;
        delete label3;
        delete label4;
        delete label5;
        delete label6;
        delete label7;
        delete label8;
        delete label9;
        delete label10;
        delete label11;
        delete label12;
        delete label13;
        delete label14;
        delete label15;
        delete label16;
        delete vpack1;
        delete vpack2;
        delete vpack2_1;
        delete vpack2_2;
        delete hpack1;
        delete hpack2;
        delete vpack;
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
