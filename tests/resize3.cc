// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

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

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "yacurs.h"

// Used when preloading libtestpreload.so
extern "C" int __test_wgetch(void*) {
    return 'q';
}

class MyWindow: public YACURS::Window {
    protected:
        void resize_handler(YACURS::Event& _e) {
            YACURS::Window::resize_handler(_e);

            YACURS::EventEx<YACURS::Size>& winch = dynamic_cast<YACURS::EventEx<YACURS::Size>&>(_e);
            std::string status_msg("YACURS::Size: rows=");

            char buff[32];
            snprintf(buff,32,"%d",winch.data().rows());
            status_msg+=buff;

            status_msg+=" cols=";

            snprintf(buff,32,"%d",winch.data().cols());
            status_msg+=buff;

            YACURS::Curses::statusline()->push_msg(status_msg);
        }

    public:
        MyWindow() : YACURS::Window() {
            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MyWindow>(YACURS::EVT_SIGWINCH,this, &MyWindow::resize_handler));
        }
        MyWindow(const YACURS::Margin& _m) : YACURS::Window(_m) {}
};

class HotKeyQuit : public YACURS::HotKey {
    public:
	HotKeyQuit(int k) : HotKey(k) {}
	HotKeyQuit(const HotKeyQuit& hk): HotKey(hk) {}

	void action() {
	    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	}

	HotKey* clone() const { return new HotKeyQuit(*this); }
};

int main() {
    try {
        YACURS::Curses::init();

        YACURS::LineObject* title = new YACURS::LineObject(YACURS::LineObject::POS_TOP,
                                           "Resize 3");
        YACURS::Curses::title(title);

        // NOTE:
        //
        // The order the objects are created (MyWindow, StatusLine) is
        // important here. Because MyWindow calls
        // StatusLine::put_msg() on resize we have to make sure
        // StatusLine is resized first. Since YACURS::EventQueue calls the
        // last YACURS::EventConnector connected first, StatusLine has to be
        // created AFTER MyWindow.

        MyWindow* w1 = new MyWindow(YACURS::Margin(1,0,1,0));
        w1->frame(true);
	w1->add_hotkey(HotKeyQuit('q'));
	w1->add_hotkey(HotKeyQuit('Q'));

        YACURS::StatusLine* sl = new YACURS::StatusLine();
        YACURS::Curses::statusline(sl);
        sl->push_msg("Press Q to quit");

        YACURS::Curses::mainwindow(w1);

        YACURS::Curses::run();

        delete title;
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
