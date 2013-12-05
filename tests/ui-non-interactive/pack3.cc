// $Id$
//
// Test Label size change while displaying.
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
#ifdef YACURS_USE_WCHAR
        label1->label("B ï g   N € w    L a b € l ¹");
#else
        label1->label("B i g   N e w    L a b e l 1");
#endif
        alarm(2);
        break;

    case 1:
#ifdef YACURS_USE_WCHAR
        label1->label("Small lab€l¹");
#else
        label1->label("Small label1");
#endif
        alarm(2);
        break;

    case 2:
#ifdef YACURS_USE_WCHAR
        label1->label("T h i s   s h o u l d   r € s i z €");
#else
        label1->label("T h i s   s h o u l d   r e s i z e");
#endif
        alarm(2);
        break;

    case 3:
#ifdef YACURS_USE_WCHAR
        label3->label("H  u  g  €    B  i  g    L  a  b  €  l  ³");
#else
        label3->label("H  u  g  e     B  i  g     L  a  b  e  l   3");
#endif
        alarm(2);
        break;

    default:
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
        break;
    }
}

int
main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO)!=1 ||
	isatty(STDIN_FILENO)!=1) exit(77);

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
            "Pack 3: Widget resizes");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0) );
        w1->frame(true);

        YACURS::HPack* hpack = new YACURS::HPack;

        YACURS::VPack* vpack1 = new YACURS::VPack;
        YACURS::VPack* vpack2 = new YACURS::VPack;

        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

#ifdef YACURS_USE_WCHAR
        label1 = new YACURS::Label("T€st lab€ļ");
#else
        label1 = new YACURS::Label("Test label");
#endif

        vpack1->add_front(label1);

#ifdef YACURS_USE_WCHAR
        YACURS::Label* label2 = new YACURS::Label("T€st lab€ļ²");
#else
        YACURS::Label* label2 = new YACURS::Label("Test label2");
#endif
        vpack1->add_back(label2);

#ifdef YACURS_USE_WCHAR
        label3 = new YACURS::Label("T€st lab€ļ³");
        YACURS::Label* label4 = new YACURS::Label("T€st lab€ļ⁴");
#else
        label3 = new YACURS::Label("Test label3");
        YACURS::Label* label4 = new YACURS::Label("Test label4");
#endif
        vpack2->add_back(label3);
        vpack2->add_back(label4);

#ifdef YACURS_USE_WCHAR
        label1->label("N€w T€st Lab€l");
        label2->label("N€w T€st Lab€l²");
        label3->label("N€w T€st Lab€l³");
        label4->label("N€w T€st Lab€l⁴");
#else
        label1->label("New Test Label");
        label2->label("New Test Label2");
        label3->label("New Test Label3");
        label4->label("New Test Label4");
#endif

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
