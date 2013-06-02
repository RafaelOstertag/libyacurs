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

// Used when preloading libtestpreload.so
#ifdef USE_WCHAR
wint_t
#else
int
#endif
__test_data[] = {
    // Remove buttons by removing them
    '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n',
    // Select AGAIN
    '\t', '\n', 0
};

#ifdef USE_WCHAR
wint_t
#else
int
#endif
__test_data2[] = {
    '\n', 0
};

extern "C" int
__test_wgetch(void*) {
#ifdef USE_WCHAR
    static wint_t round = 0;
    static wint_t* ptr2 = __test_data;
#else
    static int round = 0;
    static int* ptr2 = __test_data;
#endif

    if (*ptr2 == 0 && round < 500) {
        ptr2 = __test_data;
        round++;
    }

    if (round >= 500 && *ptr2 == 0) {
        ptr2 = __test_data2;
    }

    usleep(100);

    if (*ptr2 == 0) {
        abort();
    }

    return *ptr2++;
}

class MainWindow : public YACURS::Window {
    private:
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack1;
        YACURS::HPack* hpack2;
        YACURS::HPack* hpack3;
        YACURS::Button* button1;
        YACURS::Button* button2;
        YACURS::Button* button3;
        YACURS::Button* button4;
        YACURS::Button* button5;
        YACURS::Button* button6;
        YACURS::Button* button7;
        YACURS::Button* button8;
        YACURS::Button* button9;
        YACURS::Button* bagain;
        YACURS::Button* bquit;

        void add_buttons() {
            hpack1->add_back(button1);
            hpack1->add_back(button2);
            hpack1->add_back(button3);
            hpack1->add_back(button4);
            hpack1->add_back(button5);
            hpack2->add_back(button6);
            hpack2->add_back(button7);
            hpack2->add_back(button8);
            hpack2->add_back(button9);
        }

    protected:
        void button_press_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e =
                dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data() == button1) {
                hpack1->remove(button1);
            }

            if (e.data() == button2) {
                hpack1->remove(button2);
            }

            if (e.data() == button3) {
                hpack1->remove(button3);
            }

            if (e.data() == button4) {
                hpack1->remove(button4);
            }

            if (e.data() == button5) {
                hpack1->remove(button5);
            }

            if (e.data() == button6) {
                hpack2->remove(button6);
            }

            if (e.data() == button7) {
                hpack2->remove(button7);
            }

            if (e.data() == button8) {
                hpack2->remove(button8);
            }

            if (e.data() == button9) {
                hpack2->remove(button9);
            }

            if (e.data() == bagain) {
                add_buttons();
                hpack3->remove(bagain);
                return;
            }

            if (hpack2->widgets() == 0 &&
                hpack1->widgets() == 0)
                hpack3->add_front(bagain);

            if (e.data() == bquit) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow() : YACURS::Window(YACURS::Margin(1, 0, 1, 0) ) {
            button1 = new YACURS::Button("Button1");
            button2 = new YACURS::Button("Button2");
            button3 = new YACURS::Button("Button3");
            button4 = new YACURS::Button("Button4");
            button5 = new YACURS::Button("Button5");
            button6 = new YACURS::Button("Button6");
            button7 = new YACURS::Button("Button7");
            button8 = new YACURS::Button("Button8");
            button9 = new YACURS::Button("Button9");

            bagain = new YACURS::Button("AGAIN");
            bquit = new YACURS::Button("Quit");
            vpack1 = new YACURS::VPack();
            hpack1 = new YACURS::HPack();
            hpack2 = new YACURS::HPack();
            hpack3 = new YACURS::HPack();

            add_buttons();

            hpack3->add_back(bquit);
            vpack1->add_back(hpack1);
            vpack1->add_back(hpack2);
            vpack1->add_back(hpack3);

            widget(vpack1);

            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                                  MainWindow>(YACURS::
                                                              EVT_BUTTON_PRESS,
                                                              this,
                                                              &MainWindow::
                                                              button_press_handler) );
        }

        ~MainWindow() {
            delete button1;
            delete button2;
            delete button3;
            delete button4;
            delete button5;
            delete button6;
            delete button7;
            delete button8;
            delete button9;
            delete bquit;
            delete bagain;
            delete hpack1;
            delete hpack2;
            delete hpack3;
            delete vpack1;

            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
                                                     MainWindow>(
                                                     YACURS::EVT_BUTTON_PRESS,
                                                     this,
                                                     &MainWindow::
                                                     button_press_handler) );
        }
};

int
main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef USE_WCHAR
    setlocale(LC_ALL, "");
#endif

    try {
        YACURS::Curses::init();

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   "Packremove") );
        YACURS::Curses::statusbar(new YACURS::StatusBar);

        YACURS::Curses::mainwindow(new MainWindow);
        YACURS::Curses::mainwindow()->frame(true);

        YACURS::Curses::run();

        delete YACURS::Curses::mainwindow();
        delete YACURS::Curses::title();
        delete YACURS::Curses::statusbar();

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
