// $Id$
//
// Test functionality of CursorBuffer

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "unistd.h"

#include <iostream>

#include "yacurs.h"

void
test1(uint16_t w) {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 0, 10, w + 2) );

    win.box();

    YACURS::INTERNAL::CursorBuffer buff("abcdefghijklmnopqrstuvwxyz");

    curs_set(1);
    for (int i = 0; i < 28; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.forward_step();
    }

    for (int i = 0; i < 28; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.back_step();
    }

    for (int i = 0; i < 10; i++) {
        int16_t curs_pos;

        buff.end();
        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );
        win.refresh();

        usleep(50000);

        buff.home();
        str = YACURS::CurStr(buff.string(w,
                                         &curs_pos),
                             YACURS::Coordinates(1, 1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );
        win.refresh();

        usleep(50000);
    }

    buff.end();
    for (int i = 0; i < 30; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.backspace();
    }

    assert(buff.string().empty() );
    assert(buff.wstring().empty() );

    buff.set(L"abcdefghijklmnopqrstuvwxyz");
    for (int i = 0; i < 30; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.del ();
    }

    assert(buff.string().empty() );
    assert(buff.wstring().empty() );

    wchar_t ins_str[] = L"abcdefghijklmnopqrstuvwxyz";
    wchar_t* wptr = ins_str;

    do {
        buff.insert(*wptr++);

        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);
    }   while (*wptr != L'\0');

    assert(buff.wstring() == L"abcdefghijklmnopqrstuvwxyz");
    assert(buff.string() == "abcdefghijklmnopqrstuvwxyz");

    buff.home();
    wptr = ins_str;
    do {
        buff.insert(*wptr++);

        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);
    }   while (*wptr != L'\0');

    curs_set(0);
}

void
test2(uint16_t w) {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 0, 10, w + 2) );

    win.box();

    YACURS::INTERNAL::CursorBuffer buff("äbÄcdëËfghijklmnöpqrstüvwxÿŸzàéè€¼½");

    curs_set(1);
    for (int i = 0; i < 34; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.forward_step();
    }

    for (int i = 0; i < 34; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.back_step();
    }

    for (int i = 0; i < 10; i++) {
        int16_t curs_pos;

        buff.end();
        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );
        win.refresh();

        usleep(50000);

        buff.home();
        str = YACURS::CurStr(buff.string(w,
                                         &curs_pos),
                             YACURS::Coordinates(1, 1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );
        win.refresh();

        usleep(50000);
    }

    buff.end();
    for (int i = 0; i < 35; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.backspace();
    }

    assert(buff.string().empty() );
    assert(buff.wstring().empty() );

    buff.set(L"äbÄcdëËfghijklmnöpqrstüvwxÿŸzàéè€¼½");
    for (int i = 0; i < 35; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);

        buff.del ();
    }

    assert(buff.string().empty() );
    assert(buff.wstring().empty() );

    wchar_t ins_str[] = L"äbÄcdëËfghijklmnöpqrstüvwxÿŸzàéè€¼½";
    wchar_t* wptr = ins_str;

    do {
        buff.insert(*wptr++);

        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);
    }   while (*wptr != L'\0');

    assert(buff.wstring() == L"äbÄcdëËfghijklmnöpqrstüvwxÿŸzàéè€¼½");
    assert(buff.string() == "äbÄcdëËfghijklmnöpqrstüvwxÿŸzàéè€¼½");

    buff.home();
    wptr = ins_str;
    do {
        buff.insert(*wptr++);

        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        usleep(50000);
    }   while (*wptr != L'\0');

    curs_set(0);
}

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
#ifdef USE_WCHAR
    setlocale(LC_ALL, "");
#endif

    YACURS::Curses::init();

    for (int i = 8; i < 30; i = i + 5) {
        test1(i);
        sleep(1);
        wclear(stdscr);
        wrefresh(stdscr);
    }

#ifdef USE_WCHAR
    for (int i = 8; i < 30; i = i + 5) {
        test2(i);
        sleep(1);
        wclear(stdscr);
        wrefresh(stdscr);
    }
#endif

    YACURS::Curses::end();

    return 0;
}
