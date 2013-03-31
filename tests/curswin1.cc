// $Id$
//
// Test functionality of CursWin

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "unistd.h"

#include "yacurs.h"

void test1(YACURS::INTERNAL::CursWin w,
	   YACURS::INTERNAL::CurStr str) {
   w<<str;
   w.refresh();
   sleep(2);
   w.clear();
   w.set_box();
   w<<str;
   w.refresh();
   sleep(2);
}

int main() {
#if 1
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    Curses::init();

    YACURS::INTERNAL::CurStr str1("Hello, world!");
    YACURS::INTERNAL::CursWin win1(Area(3,2,15,70));

    win1.create();

    test1(win1, str1);
    
    YACURS::INTERNAL::CursWin subwin1(win1.derwin(Area(2,2,10,10)));
    subwin1.set_box();
    subwin1.refresh();
    sleep(2);

    Curses::end();

}
