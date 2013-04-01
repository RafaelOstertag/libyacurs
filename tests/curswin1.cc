// $Id$
//
// Test functionality of CursWin

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "unistd.h"

#include "yacurs.h"

void test1() {
    YACURS::INTERNAL::CurStr str1("Hello, world!");
    YACURS::INTERNAL::CursWin win1(Area(3,2,15,70));

    win1.create();

    win1<<str1;
    win1.refresh();
    sleep(2);

    win1.clear();
    win1.box();
    win1<<str1;
    win1.refresh();
    sleep(2);
    
    YACURS::INTERNAL::CursWin* subwin1=win1.derwin(Area(2,2,10,10));
    subwin1->box();
    subwin1->refresh();
    sleep(2);

    win1.clear();
    subwin1->clear();
    
    delete subwin1;
}

void test2() {
    YACURS::INTERNAL::CursWin win1(Area(0,0,10,10));
    for (int i=0; i<10; i++) {
	YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
				     Coordinates(i,i));
	win1.addstrx(str);
    }
    win1.refresh();
    sleep(2);
    win1.clear();
}

int main() {
#if 1
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    Curses::init();

    //   test1();
    //    sleep(1);
    test2();

    Curses::end();

}
