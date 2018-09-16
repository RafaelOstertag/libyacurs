// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif  // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif  // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif  // HAVE_STROPTS_H

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#else  // HAVE_TERMIOS_H
#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif  // HAVE_SYS_TERMIOS_H
#endif  // HAVE_TERMIOS_H

#include <iostream>
#include <sstream>

#include "yacurs.h"

// Used when preloading libtestpreload.so
#ifdef YACURS_USE_WCHAR
extern "C" int __test_wget_wch(void*, wint_t* i) {
    *i = 'q';
    return OK;
}
#else
extern "C" int __test_wgetch(void*) { return 'q'; }
#endif

class MyWindow : public YACURS::Window {
   protected:
    void resize_handler(YACURS::Event& _e) {
        YACURS::Window::resize_handler(_e);

        YACURS::EventEx<YACURS::Size>& winch =
            dynamic_cast<YACURS::EventEx<YACURS::Size>&>(_e);
        std::string status_msg("YACURS::Size: rows=");

        char buff[32];

        snprintf(buff, 32, "%d", winch.data().rows());
        status_msg += buff;

        status_msg += " cols=";

        snprintf(buff, 32, "%d", winch.data().cols());
        status_msg += buff;

        YACURS::Curses::statusbar()->push(status_msg);
    }

   public:
    MyWindow(const YACURS::Margin& m) : YACURS::Window(m) {
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<MyWindow>(YACURS::EVT_SIGWINCH, this,
                                                    &MyWindow::resize_handler));
    }
};

class HotKeyQuit : public YACURS::HotKey {
   public:
    HotKeyQuit(int k) : HotKey(k) {}

    HotKeyQuit(const HotKeyQuit& hk) : HotKey(hk) {}

    void action() { YACURS::EventQueue::submit(YACURS::EVT_QUIT); }

    HotKey* clone() const { return new HotKeyQuit(*this); }
};

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL, "en_US.UTF-8") == 0) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title =
            new YACURS::TitleBar(YACURS::TitleBar::POS_TOP, "Resize 4");
        YACURS::Curses::title(title);

        // NOTE:
        //
        // The order the objects are created (MyWindow, StatusBar) is
        // important here. Because MyWindow calls
        // StatusBar::put_msg() on resize we have to make sure
        // StatusBar is resized first. Since YACURS::EventQueue calls the
        // last YACURS::EventConnector connected first, StatusBar has to be
        // created AFTER MyWindow.

        MyWindow* w1 = new MyWindow(YACURS::Margin(1, 0, 1, 0));
        w1->frame(true);
        w1->add_hotkey(HotKeyQuit('q'));
        w1->add_hotkey(HotKeyQuit('Q'));

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        sl->push("Press Q to quit");
        YACURS::Curses::statusbar(sl);

        YACURS::HPack* hpack = new YACURS::HPack;
#ifdef YACURS_USE_WCHAR
        YACURS::Label* hl1 = new YACURS::Label("åƀčđėḟǥħıjķł₥ñøṗqṙşŧūvẇ×¥ƶ");
        YACURS::Label* hl2 = new YACURS::Label("ÅßČĐĚḞĠḢİJĶŁṀŅØṖQṘṠṪǓVẆẊ¥Ƶ");
#else
        YACURS::Label* hl1 = new YACURS::Label("abcdefghijklmnopqrstuvwxyz");
        YACURS::Label* hl2 = new YACURS::Label("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
#endif
        YACURS::Label* hl3 = new YACURS::Label("0123456789");

        hpack->add_front(hl1);
        hpack->add_front(hl2);
        hpack->add_back(hl3);

        YACURS::VPack* vpack = new YACURS::VPack;
        YACURS::Label* vls[20];

        for (int i = 0; i < 20; i++) {
            std::ostringstream _i;
            _i << i;
            vls[i] = new YACURS::Label("VYACURS::Label " + _i.str());
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
