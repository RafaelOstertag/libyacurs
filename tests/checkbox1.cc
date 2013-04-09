// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_TERMIOS_H
# include <termios.h>
#else // HAVE_TERMIOS_H
# ifdef HAVE_SYS_TERMIOS_H
#  include <sys/termios.h>
# endif // HAVE_SYS_TERMIOS_H
#endif // HAVE_TERMIOS_H

#include <iostream>
#include <sstream>

#include "yacurs.h"

// Used when preloading libtestpreload.so
int __test_data[]= {
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_UP,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    'Q',
    0
};

extern "C" int __test_wgetch(void*) {
    static int* ptr2=__test_data;

    usleep(10000);

    if (*ptr2==0) {
        abort();
    }

    return *ptr2++;
}


void key_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_KEY);

    YACURS::EventEx<int>& _ek = dynamic_cast<YACURS::EventEx<int>&>(_e);

    switch (_ek.data()) {
    case 'q':
    case 'Q':
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
        break;

    default:
        break;
    }
}

int main() {
    std::list<std::string> items;

    for (int i=0; i<24; i++) {
        std::ostringstream n;
        n<<i;
        items.push_back("Long Name ListBox Item Number " + n.str());
    }

    try {
        YACURS::Curses::init();

        YACURS::LineObject* title = new YACURS::LineObject(YACURS::LineObject::POS_TOP,
                                           "ListBox 1");
        YACURS::Curses::title(title);

        // NOTE:
        //
        // The order the objects are created (MyWindow, StatusLine) is
        // important here. Because MyWindow calls
        // StatusLine::put_msg() on resize we have to make sure
        // StatusLine is resized first. Since YACURS::EventQueue calls the
        // last YACURS::EventConnector connected first, StatusLine has to be
        // created AFTER MyWindow.

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1,0,1,0));
        w1->frame(true);

        std::vector<std::string> items;
        items.push_back("Item 1");
        items.push_back("Item 2");
        items.push_back("Item 3");
        items.push_back("Item 5");
        items.push_back("Item 6");
        items.push_back("Item 7");
        items.push_back("Item 8");
        items.push_back("Item 9");
        items.push_back("Item 10");
        YACURS::CheckBox* ckbx1 = new YACURS::CheckBox("Items12345", items);

        w1->widget(ckbx1);

        YACURS::StatusLine* sl = new YACURS::StatusLine();
        YACURS::Curses::statusline(sl);
        sl->push_msg("Press Q to quit");

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(YACURS::EVT_KEY,&key_handler));

        YACURS::Curses::run();

        delete title;
        delete ckbx1;
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
