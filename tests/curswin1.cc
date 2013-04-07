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
    YACURS::INTERNAL::CursWin win(YACURS::Area(3,2,15,70));

    win<<str1;
    win.refresh();
    sleep(2);

    win.clear();
    win.box();
    YACURS::INTERNAL::CurStr str2("Hello, world!", YACURS::Coordinates(1,1));
    win<<str2;
    win.refresh();
    sleep(2);

    YACURS::INTERNAL::CursWin* subwin=win.derwin(YACURS::Area(2,2,10,10));
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
    YACURS::INTERNAL::CursWin win(YACURS::Area(0,0,10,10));

    for (int i=0; i<10; i++) {
        YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                     YACURS::Coordinates(i,i));
        win.addstrx(str);
    }

    win.refresh();
}

void
test3() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0,10,10,10));
    win.box();

    for (int i=1; i<10; i++) {
        YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                     YACURS::Coordinates(i,i));
        win.addstrx(str);
    }

    win.refresh();
}

void
test4() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0,20,10,10));

    char chr='A';

    for (int y=0; y<10; y++) {
        win.move(YACURS::Coordinates(0,y));

        for (int x=0; x<10; x++) {
            win.addch(chr);
            usleep(10000);
            win.refresh();
        }

        chr++;
    }

    for (int y=9; y>=0; y--) {
        for (int x=9; x>=0; x--) {
            win.mvdelch(YACURS::Coordinates(x,y));
            usleep(10000);
            win.refresh();
        }
    }
}

void
test5() {
    for (int anic=0; anic<5; anic++) {
        YACURS::INTERNAL::CursWin win1(YACURS::Area(0,0,10,10));
	win1.set_color(YACURS::MESSAGEBOX_TITLE);

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         YACURS::Coordinates(i,9-i));
            win1.addstrx(str);
            usleep(50000);
            win1.refresh();
        }


        YACURS::INTERNAL::CursWin win2(YACURS::Area(10,0,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         YACURS::Coordinates(i,i));
            win2.addstrx(str);
            usleep(50000);
            win2.refresh();
        }

        YACURS::INTERNAL::CursWin win4(YACURS::Area(10,10,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         YACURS::Coordinates(0,i));
            win4.addnstr(str,9-i);
            usleep(50000);
            win4.refresh();
        }

        YACURS::INTERNAL::CursWin win3(YACURS::Area(0,10,10,10));

        for (int i=0; i<10; i++) {
            YACURS::INTERNAL::CurStr str("abcdefghiklmnopqrstuvwxyz",
                                         YACURS::Coordinates(0,i));
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
    YACURS::INTERNAL::CursWin win(YACURS::Area(0,0,10,20));

    std::string str("abcdefghijklmnopqrstuvwxyz");

    for (int i=0; i<10; i++) {
        win.move(YACURS::Coordinates(0,i));
        win.addstrx(str);
        win.refresh();
    }
}

void test7() {
    YACURS::INTERNAL::CurStr str1("abcdefgh", YACURS::Coordinates(1,1), YACURS::LISTBOX_HILITE);
    YACURS::INTERNAL::CurStr str2("abcde", YACURS::Coordinates(3,2), YACURS::LISTBOX_HILITE);

    YACURS::INTERNAL::CursWin win1(YACURS::Area(0,10,10,10));
    win1.box();
    win1.addlinex(str1);
    win1.addlinex(str2);
    win1.move(YACURS::Coordinates(0,3));
    win1.addlinex("abcdefgh");
    win1.refresh();

    YACURS::INTERNAL::CursWin win2(YACURS::Area(10,10,10,10));
    win2.addlinex(str1);
    win2.addlinex(str2);
    win2.move(YACURS::Coordinates(0,3));
    win2.addlinex("abcdefgh");
    win2.refresh();
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    YACURS::Curses::init();

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

    test7();

    sleep(1);

    YACURS::Curses::end();

}
