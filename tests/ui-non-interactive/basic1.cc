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

YACURS::TitleBar* title = 0;

void alrm(YACURS::Event& _e) {
    static int counter = 0;

    assert(_e == YACURS::EVT_SIGALRM);
    assert(title != 0);

    switch (counter) {
        case 0:
            counter++;
            title->alignment(YACURS::TitleBar::RIGHT);
            alarm(1);
            break;

        case 1:
            counter++;
            title->alignment(YACURS::TitleBar::CENTER);
            alarm(1);
            break;

        case 2:
            counter++;
            title->alignment(YACURS::TitleBar::LEFT);
            alarm(1);
            break;

        default:
            YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
            break;
    }
}

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL, "en_US.UTF-8") == 0) exit(77);
#endif

    try {
        YACURS::Curses::init();

#ifdef YACURS_USE_WCHAR
        title = new YACURS::TitleBar(YACURS::TitleBar::POS_TOP, "Basıc ¹");
#else
        title = new YACURS::TitleBar(YACURS::TitleBar::POS_TOP, "Basic 1");
#endif
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 0, 0));
        w1->frame(true);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorFunction1(YACURS::EVT_SIGALRM, &alrm));

        alarm(1);
        YACURS::Curses::run();

        delete title;
        delete w1;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
