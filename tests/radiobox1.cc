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
#ifdef USE_WCHAR
wint_t
#else
int
#endif
__test_data[] = {
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
    ' ', KEY_DOWN,
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

#ifdef USE_WCHAR
extern "C" int
__test_wget_wch(void*, wint_t* i) {
    static wint_t* ptr2 = __test_data;

    usleep(20000);

    if (*ptr2 == 0) {
        abort();
    }

    *i=*ptr2++;

    return OK;
}
#else
extern "C" int
__test_wgetch(void*) {
    static int* ptr2 = __test_data;

    usleep(10000);

    if (*ptr2 == 0) {
        abort();
    }

    return *ptr2++;
}
#endif

class HotKeyQuit : public YACURS::HotKey {
    public:
        HotKeyQuit(int k) : HotKey(k) {
        }

        HotKeyQuit(const HotKeyQuit& hk) : HotKey(hk) {
        }

        void action() {
            YACURS::EventQueue::submit(YACURS::EVT_QUIT);
        }

        HotKey* clone() const {
            return new HotKeyQuit(*this);
        }
};

int
main() {
#ifdef USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==NULL) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "ListBox 1");
        YACURS::Curses::title(title);

        // NOTE:
        //
        // The order the objects are created (MyWindow, StatusBar) is
        // important here. Because MyWindow calls
        // StatusBar::put_msg() on resize we have to make sure
        // StatusBar is resized first. Since YACURS::EventQueue calls the
        // last YACURS::EventConnector connected first, StatusBar has to be
        // created AFTER MyWindow.

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);
        w1->add_hotkey(HotKeyQuit('q') );
        w1->add_hotkey(HotKeyQuit('Q') );

        std::vector<std::string> items;
#ifdef USE_WCHAR
        items.push_back("It€m 1");
        items.push_back("It€m 2");
        items.push_back("It€m 3");
        items.push_back("It€m 5");
        items.push_back("It€m 6");
        items.push_back("It€m 7");
        items.push_back("It€m 8");
        items.push_back("It€m 9");
        items.push_back("It€m 10");
        YACURS::RadioBox* rabx1 = new YACURS::RadioBox("It€ms¹²³⁴⁵", items);
#else
        items.push_back("Item 1");
        items.push_back("Item 2");
        items.push_back("Item 3");
        items.push_back("Item 5");
        items.push_back("Item 6");
        items.push_back("Item 7");
        items.push_back("Item 8");
        items.push_back("Item 9");
        items.push_back("Item 10");
        YACURS::RadioBox* rabx1 = new YACURS::RadioBox("Items12345", items);
#endif

        w1->widget(rabx1);

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        YACURS::Curses::statusbar(sl);
        sl->push_msg("Press Q to quit");

        YACURS::Curses::mainwindow(w1);

        YACURS::Curses::run();

        delete title;
        delete rabx1;
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
