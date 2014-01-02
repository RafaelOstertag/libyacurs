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
    YACURS::CurStr str1("Ḩ€łłø, ẇøřļđ!");
    YACURS::INTERNAL::CursWin win(YACURS::Area(3, 2, 15, 70) );

    win << str1;
    win.refresh();
    #ifdef SLOW_TESTS
	sleep(2);
#endif

    win.clear();
    win.box();
    YACURS::CurStr str2("Ḩ€łłø, ẇøřļđ!", YACURS::Coordinates(1, 1) );
    win << str2;
    win.refresh();
    #ifdef SLOW_TESTS
	sleep(2);
#endif

    YACURS::INTERNAL::CursWin* subwin =
        win.derwin(YACURS::Area(2, 2, 10, 10) );
    subwin->box();
    subwin->refresh();
    #ifdef SLOW_TESTS
	sleep(2);
#endif

    win.clear();
    subwin->clear();
    win.refresh();
    delete subwin;
}

void
test2() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 0, 10, 10) );

    for (int i = 0; i < 10; i++) {
        YACURS::CurStr str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                           YACURS::Coordinates(i, i) );
        win.addstrx(str);
    }

    win.refresh();
}

void
test3() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 10, 10) );

    win.box();

    for (int i = 1; i < 10; i++) {
        YACURS::CurStr str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                           YACURS::Coordinates(i, i) );
        win.addstrx(str);
    }

    win.refresh();
}

void
test4() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 20, 10, 10) );

    char chr = 'A';

    for (int y = 0; y < 10; y++) {
        win.move(YACURS::Coordinates(0, y) );

        for (int x = 0; x < 10; x++) {
            win.addch(chr);
            #ifdef SLOW_TESTS
	usleep(70000);
#endif
            win.refresh();
        }

        chr++;
    }

    for (int y = 9; y >= 0; y--) {
        for (int x = 9; x >= 0; x--) {
            win.mvdelch(YACURS::Coordinates(x, y) );
            #ifdef SLOW_TESTS
	usleep(70000);
#endif
            win.refresh();
        }
    }
}

void
test5() {
    for (int anic = 0; anic < 2; anic++) {
        YACURS::INTERNAL::CursWin win1(YACURS::Area(0, 0, 10, 10) );
        win1.set_color(YACURS::DIALOG);

        for (int i = 0; i < 10; i++) {
            YACURS::CurStr str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                               YACURS::Coordinates(i, 9 - i) );
            win1.addstrx(str);
            #ifdef SLOW_TESTS
	usleep(70000);
#endif
            win1.refresh();
        }

        YACURS::INTERNAL::CursWin win2(YACURS::Area(10, 0, 10, 10) );

        for (int i = 0; i < 10; i++) {
            YACURS::CurStr str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                               YACURS::Coordinates(i, i) );
            win2.addstrx(str);
            #ifdef SLOW_TESTS
	usleep(70000);
#endif
            win2.refresh();
        }

        YACURS::INTERNAL::CursWin win4(YACURS::Area(10, 10, 10, 10) );

        for (int i = 0; i < 10; i++) {
            YACURS::CurStr str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                               YACURS::Coordinates(0, i) );
            win4.addnstr(str, 9 - i);
            #ifdef SLOW_TESTS
	usleep(70000);
#endif
            win4.refresh();
        }

        YACURS::INTERNAL::CursWin win3(YACURS::Area(0, 10, 10, 10) );

        for (int i = 0; i < 10; i++) {
            YACURS::CurStr str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                               YACURS::Coordinates(0, i) );
            win3.addnstr(str, i);
            #ifdef SLOW_TESTS
	usleep(70000);
#endif
            win3.refresh();
        }

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        win1.clear();
        win1.refresh();
        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        win2.clear();
        win2.refresh();
        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        win3.clear();
        win3.refresh();
        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        win4.clear();
        win4.refresh();
        #ifdef SLOW_TESTS
	usleep(70000);
#endif
    }
}

void
test6() {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 0, 10, 20) );

    std::string str("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ");

    for (int i = 0; i < 10; i++) {
        win.move(YACURS::Coordinates(0, i) );
        win.addstrx(str);
        win.refresh();
    }
}

void
test7() {
    YACURS::CurStr str1("åƀčđėḟǥħ", YACURS::Coordinates(1,
                                                        1),
                        YACURS::LISTBOX_HILITE);
    YACURS::CurStr str2("åƀčđė", YACURS::Coordinates(3,
                                                     2),
                        YACURS::LISTBOX_HILITE);

    YACURS::INTERNAL::CursWin win1(YACURS::Area(0, 10, 10, 10) );

    win1.box();
    win1.addlinex(str1);
    win1.addlinex(str2);
    win1.move(YACURS::Coordinates(0, 3) );
    win1.addlinex("åƀčđėḟǥħ");
    win1.refresh();

    YACURS::INTERNAL::CursWin win2(YACURS::Area(10, 10, 10, 10) );
    win2.addlinex(str1);
    win2.addlinex(str2);
    win2.move(YACURS::Coordinates(0, 3) );
    win2.addlinex("åƀčđėḟǥħ");
    win2.refresh();
}

void
test8() {
    YACURS::CurStr str1("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 0),
                        YACURS::DIALOG);
    YACURS::CurStr str2("åƀčđė",
                        YACURS::Coordinates(0, 1),
                        YACURS::DEFAULT);
    YACURS::CurStr str21("ḟǥḩíj",
                         YACURS::Coordinates(5, 1),
                         YACURS::INPUTWIDGET_FOCUS);
    YACURS::CurStr str3("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 2),
                        YACURS::INPUTWIDGET_NOFOCUS);
    YACURS::CurStr str4("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 3),
                        YACURS::INPUTWIDGET_FOCUS);
    YACURS::CurStr str5("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 4),
                        YACURS::INPUTWIDGET_HIDDEN);
    YACURS::CurStr str6("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 5),
                        YACURS::BUTTON_NOFOCUS);
    YACURS::CurStr str7("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 6),
                        YACURS::BUTTON_FOCUS);
    YACURS::CurStr str8("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 7),
                        YACURS::LISTBOX);
    YACURS::CurStr str9("åƀčđėḟǥḩíj",
                        YACURS::Coordinates(0, 8),
                        YACURS::LISTBOX_HILITE);
    YACURS::CurStr str10("åƀčđėḟǥḩíj",
                         YACURS::Coordinates(0, 9),
                         YACURS::CHECKBOXGROUP);

    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 10, 10, 20) );

    win.set_color(YACURS::DEFAULT);

    win.addlinex(str1);
    win += str2;
    win += str21;
    win.addlinex(str3);
    win += str4;
    win.addlinex(str5);
    win += str6;
    win.addlinex(str7);
    win += str8;
    win.addlinex(str9);
    win += str10;
    win.refresh();
}

void
test9() {
    YACURS::INTERNAL::CursWin win1(YACURS::Area(0, 10, 10,
                                                20), YACURS::DIALOG);

    win1.box();
    win1.addstrx(YACURS::CurStr("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                                YACURS::Coordinates(1,
                                                    1),
                                YACURS::INPUTWIDGET_FOCUS) );

    YACURS::INTERNAL::CursWin win2(YACURS::Area(5,
                                                5,
                                                10,
                                                20), YACURS::TITLEBAR);
    win2.addstrx(YACURS::CurStr("åƀčđėḟǥḩíjķł₥ñøṗqṙşŧūvẇ×¥ƶ",
                                YACURS::Coordinates(1,
                                                    1),
                                YACURS::INPUTWIDGET_FOCUS) );
    win2.box();

    win1.refresh(true);
    win2.refresh(true);
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
#else
    exit(77);
#endif

    YACURS::Curses::init();

    test1();
    #ifdef SLOW_TESTS
	sleep(1);
#endif

    test2();
    test3();
    test4();

    #ifdef SLOW_TESTS
	sleep(1);
#endif

    wclear(stdscr);
    wrefresh(stdscr);

    test5();

    #ifdef SLOW_TESTS
	sleep(1);
#endif

    wclear(stdscr);
    wrefresh(stdscr);

    test6();

    #ifdef SLOW_TESTS
	sleep(1);
#endif

    wclear(stdscr);
    wrefresh(stdscr);

    test7();

    #ifdef SLOW_TESTS
	sleep(1);
#endif

    wclear(stdscr);
    wrefresh(stdscr);

    test8();
    #ifdef SLOW_TESTS
	sleep(1);
#endif

    test9();
    #ifdef SLOW_TESTS
	sleep(1);
#endif

    YACURS::Curses::end();
}
