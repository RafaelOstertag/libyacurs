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
extern "C" int __test_wgetch(void*) {
    return 'q';
}

class MyWindow: public Window {
    private:
        Window* win;
    public:
        void show() {
            Window::show();
            win=new Window(Margin(3,3,3,3));
            win->frame(true);
            win->show();
        }

        void close() {
            assert(win!=0);
            win->close();
            delete win;
            win=0;
        }

        MyWindow(const Margin& m) : Window(m), win(0) {
        }
};

void key_handler(Event& _e) {
    assert(_e == EVT_KEY);

    EventEx<int>& _ek = dynamic_cast<EventEx<int>&>(_e);

    switch (_ek.data()) {
    case 'q':
    case 'Q':
        EventQueue::submit(Event(EVT_QUIT));
        break;

    default:
        break;
    }
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {

        Curses::init();

        LineObject* title = new LineObject(LineObject::POS_TOP,
                                           "Resize 5: Overlapping windows");
        Curses::title(title);

        // NOTE:
        //
        // The order the objects are created (MyWindow, StatusLine) is
        // important here. Because MyWindow calls
        // StatusLine::put_msg() on resize we have to make sure
        // StatusLine is resized first. Since EventQueue calls the
        // last EventConnector connected first, StatusLine has to be
        // created AFTER MyWindow.

        MyWindow* w1 = new MyWindow(Margin(1,0,1,0));
        w1->frame(true);

        StatusLine* sl = new StatusLine();
        sl->push_msg("Press Q to quit");
        Curses::statusline(sl);

        HPack* hpack = new HPack;
        Label* hl1 = new Label("abcdefghijklmnopqrstuvwxyz");
        Label* hl2 = new Label("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        Label* hl3 = new Label("0123456789");


        hpack->add_front(hl1);
        hpack->add_front(hl2);
        hpack->add_back(hl3);

        VPack* vpack = new VPack;
        Label* vls[20];

        for(int i=0; i<20; i++) {
            std::ostringstream _i;
            _i<<i;
            vls[i]=new Label("VLabel " + _i.str());
            vpack->add_back(vls[i]);
        }

        hpack->add_front(vpack);

        w1->widget(hpack);

        Curses::mainwindow(w1);

        EventQueue::connect_event(EventConnectorFunction1(EVT_KEY,&key_handler));

        Curses::run();

        delete vpack;

        for(int i=0; i<20; i++) {
            delete vls[i];
        }

        delete title;
        delete hl1;
        delete hl2;
        delete hl3;
        delete hpack;
        delete w1;
        delete sl;
        Curses::end();

    } catch (std::exception& e) {
        Curses::end();

        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
