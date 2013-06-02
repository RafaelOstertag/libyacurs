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
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 10, 10) );

    win.box();

    YACURS::CurStr str("ÄÖÜËäöüë", YACURS::Coordinates(1, 1) );
    win.addstr(str);

    win.refresh();
}

void
test2() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 15, 15) );

    win.box();

    YACURS::CurStr str("ÄÖÜËäöüë", YACURS::Coordinates(5, 5) );
    win.addstr(str);

    win.refresh();
}

void
test3() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 5, 5) );

    win.box();

    YACURS::CurStr str("ÄÖÜËäöüë", YACURS::Coordinates(2, 2) );
    win.addstrx(str);

    win.refresh();
}

void
test4() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 12, 12) );

    win.box();

    YACURS::CurStr str("ÄÖÜËäöüë", YACURS::Coordinates(0, 1) );
    win.addlinex(str);

    YACURS::CurStr str1("ÄÖÜËäöüë", YACURS::Coordinates(0,
                                                        2),
                        YACURS::DIALOG_TITLE);
    win.addlinex(str1);

    win.refresh();
}

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
#ifdef USE_WCHAR
    setlocale(LC_ALL, "");

    YACURS::Curses::init();

    test1();

    sleep(3);

    wclear(stdscr);
    wrefresh(stdscr);

    test2();

    sleep(3);

    wclear(stdscr);
    wrefresh(stdscr);

    test3();

    sleep(3);

    wclear(stdscr);
    wrefresh(stdscr);

    test4();

    sleep(3);

    wclear(stdscr);
    wrefresh(stdscr);

    YACURS::Curses::end();

    return 0;
#else
    return 77;
#endif
}
