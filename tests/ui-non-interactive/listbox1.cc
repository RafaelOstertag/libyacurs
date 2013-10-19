// $Id: dynlabel1.cc 5189 2013-06-11 20:03:17Z rafisol $
//
// Test listbox.h
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "yacurs.h"

const YACURS::EventType EVT_TEST_EVENT("test_event");

YACURS::ListBox<std::string>* testlb;

void
test_driver(YACURS::Event& _e) {
    assert(testlb!=0);

    static int step=0;
    assert(_e == EVT_TEST_EVENT);

    switch (step) {
    case 0:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling empty() on empty ListBox");
	if (testlb->empty()!=true) abort();
	break;
    case 1:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling selected_index() on empty ListBox");
	if (testlb->selected_index()!=0) abort();
	break;
    case 2:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling selected() on empty ListBox");
	if (testlb->selected() != std::string()) abort();
	break;
    case 3:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Add one item");
	testlb->add("One item added");
	break;
    case 4:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling empty() on non-empty ListBox");
	if (testlb->empty()!=false) abort();
	break;
    case 5:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling selected_index() on non-empty ListBox");
	if (testlb->selected_index()!=0) abort();
	break;
    case 6:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling selected() on non-empty ListBox");
	if (testlb->selected() != "One item added") abort();
	break;
    case 7:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Clearing Listbox");
	testlb->clear();
	break;
    case 8:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling empty() on empty ListBox");
	if (testlb->empty()!=true) abort();
	break;
    case 9:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling selected_index() on empty ListBox");
	if (testlb->selected_index()!=0) abort();
	break;
    case 10:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Calling selected() on empty ListBox");
	if (testlb->selected() != std::string()) abort();
	break;
    case 11:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Add several items iteratively");
	for (int i=0; i<50; i++) {
	    std::ostringstream I;
	    I << i;
	    testlb->add("item " + I.str() + " added");
	}
	break;
    case 12:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Check ListBox size.");
	if (testlb->count()!=50) abort();
	break;
    case 13:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Set selected item by item.");
	testlb->high_light(std::string("item 3 added"));
	if (testlb->selected_index() != 3) abort();
	break;
    case 14:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Set selected item by index.");
	testlb->high_light(49);
	if (testlb->selected_index() != 49) abort();
	if (testlb->selected() != std::string("item 49 added")) abort();
	break;
    case 15:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("High light non existent item");
	try {
	    testlb->high_light(std::string("non existing item"));
	    abort();
	} catch (std::out_of_range& ex) {
	    // good, expected
	} catch(...) {
	    // bad
	    abort();
	}
	break;
    case 16:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("High light off bounds index");
	try {
	    testlb->high_light(100);
	    abort();
	} catch (std::out_of_range& ex) {
	    // good, expected
	} catch(...) {
	    // bad
	    abort();
	}
	break;
    case 17:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("High light item 5");
	testlb->high_light(5);
	break;
    case 18:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Delete high lighted item");
	testlb->delete_selected();
	if (testlb->count() != 49) abort();
	if (testlb->selected_index() != 4) abort();
	if (testlb->selected() != std::string("item 4 added")) abort();
	break;
    case 19:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Set selected item to value"); 
	try {
	    std::string newstr("replaced by selected()");
	    testlb->selected(newstr);
	} catch (std::out_of_range& ex) {
	    abort();
	}
	if (testlb->selected() != std::string("replaced by selected()")) abort();
	break;
    case 20:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Go to just set item by index"); 
	try {
	    testlb->high_light(4);
	} catch(std::out_of_range& ex) {
	    abort();
	}
	if (testlb->selected() != std::string("replaced by selected()")) abort();
	break;
    case 21:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Go to just set item by item search"); 
	try {
	    testlb->high_light(std::string("replaced by selected()"));
	} catch (std::out_of_range& ex) {
	    abort();
	}
	if (testlb->selected() != std::string("replaced by selected()")) abort();
	if (testlb->selected_index() != 4) abort();
	break;
    case 22:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Clear list"); 
	testlb->clear();
	if (!testlb->empty()) abort();
	if (testlb->count() != 0) abort();
	break;
    case 23: {
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Assign list");
	std::list<std::string> lst;
	for (int i=0; i<10; i++) {
	    std::ostringstream I;
	    I << i;
	    lst.push_back(I.str() + " list item");
	}
	testlb->set(lst);
	if (testlb->empty()) abort();
	if (testlb->count() != 10) abort();
    }
	break;
    case 24:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Set sort order ascending");
	testlb->sort_order(YACURS::ASCENDING);
	if (testlb->sort_order()!=YACURS::ASCENDING) abort();
	try {
	    testlb->high_light(0);
	} catch (std::out_of_range) {
	    abort();
	}
	if (testlb->selected() != "0 list item") abort();
	try {
	    testlb->high_light(9);
	} catch (std::out_of_range) {
	    abort();
	}
	if (testlb->selected() != "9 list item") abort();
	break;
    case 25:
	YACURS::Curses::statusbar()->pop_msg();
	YACURS::Curses::statusbar()->push_msg("Set sort order descending");
	testlb->sort_order(YACURS::DESCENDING);
	if (testlb->sort_order()!=YACURS::DESCENDING) abort();
	try {
	    testlb->high_light(0);
	} catch (std::out_of_range) {
	    abort();
	}
	if (testlb->selected() != "9 list item") abort();
	try {
	    testlb->high_light(9);
	} catch (std::out_of_range) {
	    abort();
	}
	if (testlb->selected() != "0 list item") abort();
	break;
    case 26:
	YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	break;
    }

    step++;
    usleep(70000);
    YACURS::EventQueue::submit(EVT_TEST_EVENT);
}

int
main() {
#if 0
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
            "ListBox<std::string>");
        YACURS::Curses::title(title);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        YACURS::Curses::statusbar(sl);

	testlb = new YACURS::ListBox<std::string>();
        w1->widget(testlb);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(EVT_TEST_EVENT, &test_driver) );

	// The queue blocks while waiting for input, so simply
	// generate a signal for snapping out of wgetch().
	alarm(1);
	YACURS::EventQueue::submit(EVT_TEST_EVENT);
        YACURS::Curses::run();

        YACURS::EventQueue::disconnect_event(YACURS::EventConnectorFunction1(EVT_TEST_EVENT, &test_driver) );

        delete title;
	delete testlb;

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
