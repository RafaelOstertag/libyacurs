// $Id$
//
// Test functionality of CursWin

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "unistd.h"

#include <iostream>

#include "yacurs.h"

void
test1() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(3, 2, 15, 3) );
    win.box();
    win.vrule();
    win.refresh();

    sleep(2);
}

void
test2() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(3, 2, 3, 60) );
    win.box();
    win.hrule();
    win.refresh();

    sleep(2);
}

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==NULL) exit(77);
#endif

    YACURS::Curses::init();

    test1();

    wclear(stdscr);
    wrefresh(stdscr);

    test2();

    YACURS::Curses::end();
}
