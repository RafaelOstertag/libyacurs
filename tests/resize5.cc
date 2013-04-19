// $Id$
//
// Test resizing with overlapping windows.
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
extern "C" int
__test_wgetch(void*) {
    return 'q';
}

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

class MyWindow : public YACURS::Window {
    private:
        YACURS::Window* win;
    public:
        void show() {
            YACURS::Window::show();

            win = new YACURS::Window(YACURS::Margin(3, 3, 3, 3) );
            win->frame(true);
            win->add_hotkey(HotKeyQuit('q') );
            win->add_hotkey(HotKeyQuit('Q') );
            win->show();
        }

        void close() {
            assert(win != 0);
            win->close();
            delete win;
            win = 0;
        }

        MyWindow(const YACURS::Margin& m) : YACURS::Window(m), win(0) {
        }
};

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        YACURS::Curses::init();

        YACURS::LineObject* title = new YACURS::LineObject(
            YACURS::LineObject::POS_TOP,
            "Resize 5: Overlapping windows");
        YACURS::Curses::title(title);

        // NOTE:
        //
        // The order the objects are created (MyWindow, StatusLine) is
        // important here. Because MyWindow calls
        // StatusLine::put_msg() on resize we have to make sure
        // StatusLine is resized first. Since YACURS::EventQueue calls the
        // last YACURS::EventConnector connected first, StatusLine has to be
        // created AFTER MyWindow.

        MyWindow* w1 = new MyWindow(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);
        w1->add_hotkey(HotKeyQuit('q') );
        w1->add_hotkey(HotKeyQuit('Q') );

        YACURS::StatusLine* sl = new YACURS::StatusLine();
        sl->push_msg("Press Q to quit");
        YACURS::Curses::statusline(sl);

        YACURS::HPack* hpack = new YACURS::HPack;
        YACURS::Label* hl1 = new YACURS::Label("abcdefghijklmnopqrstuvwxyz");
        YACURS::Label* hl2 = new YACURS::Label("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        YACURS::Label* hl3 = new YACURS::Label("0123456789");

        hpack->add_front(hl1);
        hpack->add_front(hl2);
        hpack->add_back(hl3);

        YACURS::VPack* vpack = new YACURS::VPack;
        YACURS::Label* vls[20];

        for (int i = 0; i < 20; i++) {
            std::ostringstream _i;
            _i << i;
            vls[i] = new YACURS::Label("VYACURS::Label " + _i.str() );
            vpack->add_back(vls[i]);
        }

        hpack->add_front(vpack);

        w1->widget(hpack);

        YACURS::Curses::mainwindow(w1);

        YACURS::Curses::run();

        delete vpack;

        for (int i = 0; i < 20; i++) {
            delete vls[i];
        }

        delete title;
        delete hl1;
        delete hl2;
        delete hl3;
        delete hpack;
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
