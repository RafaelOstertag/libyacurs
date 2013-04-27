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
YACURS::Label* label1;
YACURS::Label* label3;

void
alrm(YACURS::Event& _e) {
    static int i = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    switch (i++) {
    case 0:
        label1->label("Big New Label1");
        alarm(1);
        break;

    case 1:
        label1->label("Small label1");
        alarm(1);
        break;

    case 2:
        label1->label("This should resize");
        alarm(1);
        break;

    case 3:
        label3->label("Huge Big Label 3");
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

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Pack 7: Always dynamic, only vertical hinting");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::VPack* vpack = new YACURS::VPack;
        YACURS::HPack* hpack = new YACURS::HPack;
        YACURS::HPack* hpack1 = new YACURS::HPack();
        YACURS::HPack* hpack2 = new YACURS::HPack();
        YACURS::VPack* vpack1 = new YACURS::VPack;
        YACURS::VPack* vpack2 = new YACURS::VPack;
        YACURS::VPack* vpack2_1 = new YACURS::VPack();
        YACURS::VPack* vpack2_2 = new YACURS::VPack();

        label1 = new YACURS::Label("Test label");
        YACURS::Label* label2 = new YACURS::Label("Test label2");
        label3 = new YACURS::Label("Test label3");
        YACURS::Label* label4 = new YACURS::Label("Test label4");
        YACURS::Label* label5 = new YACURS::Label("Label 5");
        YACURS::Label* label6 = new YACURS::Label("Label 6");
        YACURS::Label* label7 = new YACURS::Label("Label 7");
        YACURS::Label* label8 = new YACURS::Label("Label 8");
        YACURS::Label* label9 = new YACURS::Label("Label 9");
        YACURS::Label* label10 = new YACURS::Label("Label 10");
        YACURS::Label* label11 = new YACURS::Label("Label 11");
        YACURS::Label* label12 = new YACURS::Label("Label 12");
        YACURS::Label* label13 = new YACURS::Label("Label 13");
        YACURS::Label* label14 = new YACURS::Label("Label 14");
        YACURS::Label* label15 = new YACURS::Label("Label 15");
        YACURS::Label* label16 = new YACURS::Label("Label 16");

        vpack->add_front(hpack);
        vpack->add_front(hpack1);
        vpack->add_back(hpack2);

        hpack->always_dynamic(true);
        hpack->hinting(false);
        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

        hpack1->always_dynamic(true);
        hpack1->hinting(false);
        hpack1->add_back(vpack2_1);
        hpack1->add_back(vpack2_2);

        vpack1->always_dynamic(true);
        vpack1->hinting(true);
        vpack1->add_back(label1);
        vpack1->add_back(label2);
        vpack1->add_front(label3);

        vpack2->always_dynamic(true);
        vpack2->hinting(true);
        vpack2->add_back(label4);
        vpack2->add_back(label5);
        vpack2->add_back(label6);

        vpack2_1->always_dynamic(true);
        vpack2_1->hinting(true);
        vpack2_1->add_front(label7);
        vpack2_1->add_front(label8);
        vpack2_1->add_front(label9);

        vpack2_2->always_dynamic(true);
        vpack2_2->hinting(true);
        vpack2_2->add_back(label10);
        vpack2_2->add_back(label11);
        vpack2_2->add_back(label12);
        vpack2_2->add_back(label13);
        vpack2_2->add_back(label14);

        hpack2->always_dynamic(true);
        hpack2->hinting(false);
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
