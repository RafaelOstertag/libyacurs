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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        buff.home();
        str = YACURS::CurStr(buff.string(w,
                                         &curs_pos),
                             YACURS::Coordinates(1, 1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );
        win.refresh();

        #ifdef SLOW_TESTS
	usleep(70000);
#endif
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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        buff.backspace();
    }

    if (!buff.string().empty()) abort();
    if (!buff.wstring().empty()) abort();

    buff.set(L"abcdefghijklmnopqrstuvwxyz");
    for (int i = 0; i < 30; i++) {
        int16_t curs_pos;

        YACURS::CurStr str(buff.string(w, &curs_pos), YACURS::Coordinates(
                               1,
                               1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );

        win.refresh();

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif
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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif
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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

        buff.home();
        str = YACURS::CurStr(buff.string(w,
                                         &curs_pos),
                             YACURS::Coordinates(1, 1) );
        win.addlinex(str);
        win.move(YACURS::Coordinates(1 + curs_pos, 1) );
        win.refresh();

        #ifdef SLOW_TESTS
	usleep(70000);
#endif
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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif
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

        #ifdef SLOW_TESTS
	usleep(70000);
#endif
    }   while (*wptr != L'\0');

    curs_set(0);
}

void test3() {
    YACURS::INTERNAL::CursorBuffer* cb_ptr = 
	new YACURS::INTERNAL::CursorBuffer("Test Buffer");

    // Newly created CursorBuffer must not be in changed state
    if (cb_ptr->changed()) abort();

    // Must not change CursorBuffer change state, since string is less
    // than 64 characters.
    cb_ptr->max_size(64);
    if (cb_ptr->changed()) abort();

    // Change state has to be set, since string is greater than 4
    // characters.
    cb_ptr->max_size(4);
    if (!cb_ptr->changed()) abort();

    // String must be "Test" now.
    if (cb_ptr->string() != "Test") abort();

    delete cb_ptr;

    cb_ptr = new YACURS::INTERNAL::CursorBuffer("Test Buffer");

    // These methods must not change the change state of the buffer
    cb_ptr->end();
    cb_ptr->home();
    cb_ptr->forward_step();
    cb_ptr->back_step();
    if (cb_ptr->changed()) abort();

    // This function must change the change state
    cb_ptr->insert('a');
    if (!cb_ptr->changed()) abort();

    delete cb_ptr;
    cb_ptr = new YACURS::INTERNAL::CursorBuffer("Test Buffer");

    // Cursor must be at beginning of buffer, thus a backspace must
    // not change the buffer.
    cb_ptr->backspace();
    if (cb_ptr->changed()) abort();

    // This must change the buffer
    cb_ptr->end();
    cb_ptr->backspace();
    if (!cb_ptr->changed()) abort();

    delete cb_ptr;
    cb_ptr = new YACURS::INTERNAL::CursorBuffer("Test Buffer");

    // Deleting a character at the very end must not change the
    // buffer, since there is no change
    cb_ptr->end();
    cb_ptr->del();
    if (cb_ptr->changed()) abort();

    // Deleting a chracter at the beginning of the buffer must set the
    // changed state.
    cb_ptr->home();
    cb_ptr->del();
    if (!cb_ptr->changed()) abort();


    delete cb_ptr;
    cb_ptr = new YACURS::INTERNAL::CursorBuffer("Test Buffer");

    // Setting the value to the same value must not change
    // the changed state.
    cb_ptr->set("Test Buffer");
    if (cb_ptr->changed()) abort();

    // Setting the value to something different must not change the
    // changed state.
    cb_ptr->set("Test");
    if (cb_ptr->changed()) abort();

    delete cb_ptr;

    // CursorBuffer must obey maximum size when initializing
    cb_ptr = new YACURS::INTERNAL::CursorBuffer("Test Buffer", 4);

    if (cb_ptr->changed()) abort();
    if (cb_ptr->length() != 4) abort();
    if (cb_ptr->string() != "Test") abort();

    delete cb_ptr;
}

#ifdef YACURS_USE_WCHAR
void test4() {
    YACURS::INTERNAL::CursorBuffer* cb_ptr = 
	new YACURS::INTERNAL::CursorBuffer(L"T€st Buffer");

    // Newly created CursorBuffer must not be in changed state
    if (cb_ptr->changed()) abort();

    // Must not change CursorBuffer change state, since string is less
    // than 64 characters.
    cb_ptr->max_size(64);
    if (cb_ptr->changed()) abort();

    // Change state has to be set, since string is greater than 4
    // characters.
    cb_ptr->max_size(4);
    if (!cb_ptr->changed()) abort();

    // String must be "Test" now.
    if (cb_ptr->wstring() != L"T€st") abort();
    if (cb_ptr->string() != "T€st") abort();

    delete cb_ptr;

    cb_ptr = new YACURS::INTERNAL::CursorBuffer(L"T€st Buffer");

    // These methods must not change the change state of the buffer
    cb_ptr->end();
    cb_ptr->home();
    cb_ptr->forward_step();
    cb_ptr->back_step();
    if (cb_ptr->changed()) abort();

    // This function must change the change state
    cb_ptr->insert(L'ä');
    if (!cb_ptr->changed()) abort();

    delete cb_ptr;
    cb_ptr = new YACURS::INTERNAL::CursorBuffer(L"T€st Buffer");

    // Cursor must be at beginning of buffer, thus a backspace must
    // not change the buffer.
    cb_ptr->backspace();
    if (cb_ptr->changed()) abort();

    // This must change the buffer
    cb_ptr->end();
    cb_ptr->backspace();
    if (!cb_ptr->changed()) abort();

    delete cb_ptr;
    cb_ptr = new YACURS::INTERNAL::CursorBuffer(L"T€st Buffer");

    // Deleting a character at the very end must not change the
    // buffer, since there is no change
    cb_ptr->end();
    cb_ptr->del();
    if (cb_ptr->changed()) abort();

    // Deleting a chracter at the beginning of the buffer must set the
    // changed state.
    cb_ptr->home();
    cb_ptr->del();
    if (!cb_ptr->changed()) abort();


    delete cb_ptr;
    cb_ptr = new YACURS::INTERNAL::CursorBuffer(L"T€st Buffer");

    // Setting the value to the same value must not change
    // the changed state.
    cb_ptr->set(L"T€st Buffer");
    if (cb_ptr->changed()) abort();

    // Setting the value to something different must not change the
    // changed state.
    cb_ptr->set(L"T€st");
    if (cb_ptr->changed()) abort();

    delete cb_ptr;

    // CursorBuffer must obey maximum size when initializing
    cb_ptr = new YACURS::INTERNAL::CursorBuffer(L"T€st Buffer", 4);

    if (cb_ptr->changed()) abort();
    if (cb_ptr->length() != 4) abort();
    if (cb_ptr->wstring() != L"T€st") abort();
    if (cb_ptr->string() != "T€st") abort();

    delete cb_ptr;
}
#endif // YACURS_USE_WCHAR

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
#if !defined(HAVE_SETLOCALE)
# error "NLS support requested, but no setlocale() available"
#endif
    if (setlocale(LC_ALL,"en_US.UTF-8")==0) exit(77);
#endif

    test3();
#ifdef YACURS_USE_WCHAR
    test4();
#endif

    YACURS::Curses::init();

    for (int i = 8; i < 30; i = i + 5) {
        test1(i);
        #ifdef SLOW_TESTS
	sleep(1);
#endif
        wclear(stdscr);
        wrefresh(stdscr);
    }

#ifdef YACURS_USE_WCHAR
    for (int i = 8; i < 30; i = i + 5) {
        test2(i);
        #ifdef SLOW_TESTS
	sleep(1);
#endif
        wclear(stdscr);
        wrefresh(stdscr);
    }
#endif

    YACURS::Curses::end();

    return 0;
}
