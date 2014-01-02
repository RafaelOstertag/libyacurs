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

    #ifdef SLOW_TESTS
	sleep(2);
#endif
}

void
test2() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(3, 2, 15, 3) );
    win.vrule();
    win.refresh();

    #ifdef SLOW_TESTS
	sleep(2);
#endif
}

void
test3() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(3, 2, 3, 60) );
    win.box();
    win.hrule();
    win.refresh();

    #ifdef SLOW_TESTS
	sleep(2);
#endif
}

void
test4() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(3, 2, 3, 60) );
    win.hrule();
    win.refresh();

    #ifdef SLOW_TESTS
	sleep(2);
#endif
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

    YACURS::Curses::init();

    test1();

    wclear(stdscr);
    wrefresh(stdscr);

    test2();

    wclear(stdscr);
    wrefresh(stdscr);

    test3();

    wclear(stdscr);
    wrefresh(stdscr);

    test4();

    YACURS::Curses::end();
}
