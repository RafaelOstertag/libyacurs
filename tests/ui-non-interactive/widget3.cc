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
YACURS::HPack* hpack;
YACURS::VPack* vpack1;
YACURS::VPack* vpack2;

YACURS::DynLabel* label1;
YACURS::DynLabel* label2;
YACURS::Input<>* input1;
YACURS::Input<>* input2;

void
alrm(YACURS::Event& _e) {
    static int i = 0;

    assert(_e == YACURS::EVT_SIGALRM);

    switch (i++) {
    case 0:
#ifdef YACURS_USE_WCHAR
        input1->input(
            "åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ");
#else
        input1->input(
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
#endif
        break;

    case 1:
#ifdef YACURS_USE_WCHAR
        input2->input(
            "åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ");
#else
        input2->input(
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
#endif
        break;

    case 2:
#ifdef YACURS_USE_WCHAR
        label1->label(
            "åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ");
#else
        label1->label(
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
#endif
        break;

    case 3:
#ifdef YACURS_USE_WCHAR
        label2->label(
            "åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶåƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ");
#else
        label2->label(
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
#endif
        break;

    default:
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
        break;
    }

    alarm(1);
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
            "Widget 3: setting text in realized YACURS::Input/YACURS::DynLabel");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        hpack = new YACURS::HPack;
        vpack1 = new YACURS::VPack;
        vpack2 = new YACURS::VPack;

        label1 = new YACURS::DynLabel("YACURS::DynLabel1");
        input1 = new YACURS::Input<>;
        vpack1->add_back(label1);
        vpack1->add_back(input1);
        vpack1->hinting(false);

        label2 = new YACURS::DynLabel("YACURS::DynLabel2");
        input2 = new YACURS::Input<>;
        vpack2->add_back(label2);
        vpack2->add_back(input2);
        vpack2->hinting(false);

        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

        w1->widget(hpack);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(5);
        YACURS::Curses::run();

        delete title;
        delete hpack;
        delete vpack1;
        delete vpack2;
        delete label1;
        delete label2;
        delete input1;
        delete input2;
        delete w1;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
