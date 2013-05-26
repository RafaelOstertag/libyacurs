// $Id$
//
// Test functionality of CursorBuf

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "unistd.h"

#ifdef ENABLE_NLS
#include <locale.h>
#endif

#include "yacurs.h"

void test1() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 10, 10) );
    win.box();

    YACURS::INTERNAL::CursorBuf buff("abcdefghiklmnopqrstuvwxyz");


    for (int i=0; i<25; i++) {
	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(8,&curs_pos), YACURS::Coordinates(1,1));
	win.addstr(str);
	win.move((1+curs_pos,1));

	win.refresh();

	sleep(1);

	buff.forward_step();
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

    YACURS::Curses::init();
    
    test1();

    sleep(3);
    
    wclear(stdscr);
    wrefresh(stdscr);

    YACURS::Curses::end();

    return 0;
}
