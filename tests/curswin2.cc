// $Id$
//
// Test functionality of CursWin

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "unistd.h"

#ifdef ENABLE_NLS
#include <locale.h>
#endif

#include "yacurs.h"


int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");

    YACURS::Curses::init();

    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 10, 10) );
    win.box();

    YACURS::CurStr str("ÄÖÜËäöüë", YACURS::Coordinates(1,1));
    win.addstr(str);

    win.refresh();

    sleep(5);

    YACURS::Curses::end();

    return 0;
#else
    return 77;
#endif
}
