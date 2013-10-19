// $Id$
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>

#include "yacurs.h"

YACURS::Input<>* testinput;

// Used when preloading libtestpreload.so
#ifdef USE_WCHAR
wint_t
#else
int
#endif
__test_data[6][256] = {
    // Test for proper input catching (case #0)
    { 'a','b','c','d', 0 },
    // Test read only input (case #1)
    { 'a','b','c','d', 0 },
    // Test read only input again (case #2)
    { 'a','b','c','d', 0 },
    // Test unsetting read only input (case #3)
    { 'a','b','c','d', 0 },
    // Test setting max length (case #4)
    { 'a','b','c','d', 0 },
    { 0 }
};

void check_input(int step) {
    switch (step) {
    case 0:
	// Test if the string has been properly written
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Expected string written?");
	if (testinput->input() != "abcd") abort();
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Set input readonly");
	// Prepare for the read only test
	testinput->readonly(true);
	break;
    case 1:
	// Some keys have been pressed, however we're read only, so
	// the buffer must still be the same.
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Still same input?");
	if (testinput->input() != "abcd") abort();
	YACURS::Curses::statusbar()->pop_msg();
	// Clear the input buffer and let be some more keys pressed
	YACURS::Curses::statusbar()->push_msg("Clear input");
	testinput->clear();
	break;
    case 2:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Read only mode succeeded?");
	// The input must still be empty, since read-only.
	if (!testinput->input().empty()) abort();

	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Unset read only mode");
	// We unset read only, and see if key strokes come thru again
	testinput->readonly(false);
	break;
    case 3:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Unsetting read only mode succeeded?");
	// Did keystrokes come thru?
	if (testinput->input() != "abcd" ) abort();

	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Creating new input with max length");
	// Clear for the next test, which limimts the maximum length
	// of the input to 2 characters.
	testinput->max_input(2);
	testinput->clear();
	break;
    case 4:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Expected string written?");
	// Input must be exactly two characters, namely "ab"
	if (testinput->input().length() != 2) abort();
	if (testinput->input() != "ab") abort();
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Clear input");
	testinput->clear();
	break;
    default:
	abort();
    }
}

#ifdef USE_WCHAR
extern "C" int
__test_wget_wch(void*, wint_t* i) {
    static int step=0;
    static int row=0;
    static int col=0;

    usleep(70000);

    if (__test_data[row][col] == 0) {
	check_input(step++);
	col=0;

	if (__test_data[++row][col] == 0) {
	    // Bail out, no more test data
	    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	}
    }

    *i=__test_data[row][col++];

    return OK;
}
#else
extern "C" int
__test_wgetch(void*) {
    static int step=0;
    static int row=0;
    static int col=0;

    usleep(70000);

    if (__test_data[row][col] == 0) {
	check_input(step++);
	col=0;

	if (__test_data[++row][col] == 0) {
	    // Bail out, no more test data
	    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	}
    }

    return __test_data[row][col++];
}
#endif

int
main() {
#if 1
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==NULL) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Input");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        YACURS::Curses::statusbar(sl);

	testinput = new YACURS::Input<>;
        w1->widget(testinput);

        YACURS::Curses::mainwindow(w1);

        YACURS::Curses::run();

        delete title;
	delete testinput;

        delete w1;
        delete sl;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();

        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

