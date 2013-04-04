// $Id$
//
// Test functionality of CursWin

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "unistd.h"

#include "yacurs.h"

void
test1() {
    YACURS::INTERNAL::CurStr str1("Hello, world!");
    YACURS::INTERNAL::CursWin win(Area(3,2,15,70));

    win<<str1;
    win.refresh();
    sleep(2);

    win.clear();
    win.box();
    YACURS::INTERNAL::CurStr str2("Hello, world!", Coordinates(1,1));
    win<<str2;
    win.refresh();
    sleep(2);

    YACURS::INTERNAL::CursWin* subwin=win.derwin(Area(2,2,10,10));
    subwin->box();
    subwin->refresh();
    sleep(2);

    win.clear();
    subwin->clear();
    win.refresh();
    delete subwin;
}

void
test2() {
    YACURS::INTERNAL::CursWin win(Area(0,0,10,10));

    for (int i=0; i<10; i++) {
        YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                     Coordinates(i,i));
        win.addstrx(str);
    }

    win.refresh();
}

void
test3() {
    YACURS::INTERNAL::CursWin win(Area(0,10,10,10));
    win.box();

    for (int i=1; i<10; i++) {
        YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                     Coordinates(i,i));
        win.addstrx(str);
    }

    win.refresh();
}

void
test4() {
    YACURS::INTERNAL::CursWin win(Area(0,20,10,10));

    char chr='A';

    for (int y=0; y<10; y++) {
        win.move(Coordinates(0,y));

        for (int x=0; x<10; x++) {
            win.addch(chr);
            usleep(10000);
            win.refresh();
        }

        chr++;
    }

    for (int y=9; y>=0; y--) {
        for (int x=9; x>=0; x--) {
            win.mvdelch(Coordinates(x,y));
            usleep(10000);
            win.refresh();
        }
    }
}

void
test5() {
    for (int anic=0; anic<10; anic++) {
        YACURS::INTERNAL::CursWin win1(Area(0,0,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         Coordinates(i,9-i));
            win1.addstrx(str);
            usleep(50000);
            win1.refresh();
        }


        YACURS::INTERNAL::CursWin win2(Area(10,0,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         Coordinates(i,i));
            win2.addstrx(str);
            usleep(50000);
            win2.refresh();
        }

        YACURS::INTERNAL::CursWin win4(Area(10,10,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         Coordinates(0,i));
            win4.addnstr(str,9-i);
            usleep(50000);
            win4.refresh();
        }

        YACURS::INTERNAL::CursWin win3(Area(0,10,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         Coordinates(0,i));
            win3.addnstr(str,i);
            usleep(50000);
            win3.refresh();
        }

        usleep(50000);

        win1.clear();
        win1.refresh();
        usleep(50000);

        win2.clear();
        win2.refresh();
        usleep(50000);

        win3.clear();
        win3.refresh();
        usleep(50000);

        win4.clear();
        win4.refresh();
        usleep(50000);
    }
}

void
test6() {
    YACURS::INTERNAL::CursWin win(Area(0,0,10,20));

    std::string str("abcdefghijklmnopqrstuvwxyz");

    for (int i=0; i<10; i++) {
        win.move(Coordinates(0,i));
        win.addstrx(str);
        win.refresh();
    }
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    Curses::init();

    test1();
    sleep(1);

    test2();
    test3();
    test4();

    sleep(1);

    wclear(stdscr);
    wrefresh(stdscr);

    test5();

    sleep(1);

    wclear(stdscr);
    wrefresh(stdscr);

    test6();

    sleep(1);

    Curses::end();

}
