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

void test(uint16_t w) {
    YACURS::INTERNAL::CursWin win(YACURS::Area(0, 0, 10, w+2) );
    win.box();

    YACURS::INTERNAL::CursorBuf buff("abcdefghijklmnopqrstuvwxyz");

    curs_set(1);
    for (int i=0; i<28; i++) {
	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));

	win.refresh();

	usleep(50000);

	buff.forward_step();
    }

    for (int i=0; i<28; i++) {
	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));

	win.refresh();

	usleep(50000);

	buff.back_step();
    }

    for (int i=0; i<10; i++) {
	int16_t curs_pos;
	
	buff.end();
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));
	win.refresh();

	usleep(50000);

	buff.home();
	str=YACURS::CurStr(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));
	win.refresh();

	usleep(50000);
    }

    buff.end();
    for (int i=0; i<30; i++) {
	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));

	win.refresh();

	usleep(50000);

	buff.backspace();
    }

    assert(buff.buffer_string().empty());
    assert(buff.buffer_wstring().empty());

    buff.buffer(L"abcdefghijklmnopqrstuvwxyz");
    for (int i=0; i<30; i++) {
	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));

	win.refresh();

	usleep(50000);

	buff.delete_char();
    }

    assert(buff.buffer_string().empty());
    assert(buff.buffer_wstring().empty());

    wchar_t ins_str[] = L"abcdefghijklmnopqrstuvwxyz";
    wchar_t* wptr=ins_str;

    do {
	buff.insert(*wptr++);

	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));

	win.refresh();

	usleep(50000);
    }	while (*wptr!=L'\0');

    assert(buff.buffer_wstring() == L"abcdefghijklmnopqrstuvwxyz");
    assert(buff.buffer_string() == "abcdefghijklmnopqrstuvwxyz");

    buff.home();
    wptr=ins_str;
    do {
	buff.insert(*wptr++);

	int16_t curs_pos;
	
	YACURS::CurStr str(buff.get_string(w,&curs_pos), YACURS::Coordinates(1,1));
	win.addlinex(str);
	win.move(YACURS::Coordinates(1+curs_pos,1));

	win.refresh();

	usleep(50000);
    }	while (*wptr!=L'\0');

    curs_set(0);
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
    
    for (int i=8; i<30; i++) {
	test(i);
	sleep(1);
	wclear(stdscr);
	wrefresh(stdscr);
    }

    YACURS::Curses::end();

    return 0;
}
