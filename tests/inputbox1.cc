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
int __test_data[] = {
    // Open dialog
    '\n',
    // enter text
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // Ok, dialog
    '\t', '\n',
    // Open Dialog
    '\n',
    // enter text
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // cancel dialog
    '\t', '\t', '\n',
    // Quit app
    '\t', '\n', 0
};

extern "C" int
__test_wgetch(void*) {
    static int* ptr2 = __test_data;

    usleep(70000);

    if (*ptr2 == 0) {
        abort();
    }

    return *ptr2++;
}

class MainWindow : public YACURS::Window {
    private:
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack1;
        YACURS::Button* button1;
        YACURS::Button* button2;
        YACURS::Label* label1;
        YACURS::InputBox* inputbox1;

    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt =
                dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (inputbox1 != 0 && evt.data() == inputbox1) {
                YACURS::Curses::statusline()->push_msg("Dialog 1 closed");

                if (inputbox1->dialog_state() == YACURS::Dialog::DIALOG_OK)
                    label1->label("DIALOG_OK");
                else
                    label1->label("DIALOG_CANCEL");

                delete inputbox1;
                inputbox1 = 0;
            }
        }

        void button_press_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e =
                dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data() == button1) {
                assert(inputbox1 == 0);

                inputbox1 = new YACURS::InputBox("Test Dialog", "Enter text");
                inputbox1->show();
                return;
            }

            if (e.data() == button2) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow() : YACURS::Window(YACURS::Margin(1, 0, 1, 0) ), inputbox1(
                0) {
            button1 = new YACURS::Button("New Window");
            button2 = new YACURS::Button("Quit");
            vpack1 = new YACURS::VPack;
            hpack1 = new YACURS::HPack;
            label1 = new YACURS::Label("dialog state");
            hpack1->add_back(button1);
            hpack1->add_back(button2);
            vpack1->add_front(label1);
            vpack1->add_back(hpack1);
            widget(vpack1);

            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                                  MainWindow>(YACURS::
                                                              EVT_BUTTON_PRESS,
                                                              this,
                                                              &MainWindow::
                                                              button_press_handler) );
            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                                  MainWindow>(YACURS::
                                                              EVT_WINDOW_CLOSE,
                                                              this,
                                                              &MainWindow::
                                                              window_close_handler) );
        }

        ~MainWindow() {
            if (inputbox1)
                delete inputbox1;

            delete button1;
            delete button2;
            delete hpack1;
            delete vpack1;
            delete label1;

            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
                                                     MainWindow>(
                                                     YACURS::EVT_BUTTON_PRESS,
                                                     this,
                                                     &MainWindow::
                                                     button_press_handler) );
            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
                                                     MainWindow>(
                                                     YACURS::EVT_WINDOW_CLOSE,
                                                     this,
                                                     &MainWindow::
                                                     window_close_handler) );
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

        YACURS::Curses::title(new YACURS::LineObject(YACURS::LineObject::
                                                     POS_TOP,
                                                     "YACURS::InputBox 1") );
        YACURS::Curses::statusline(new YACURS::StatusLine);

        YACURS::Curses::mainwindow(new MainWindow);
        YACURS::Curses::mainwindow()->frame(true);

        YACURS::Curses::run();

        delete YACURS::Curses::mainwindow();
        delete YACURS::Curses::title();
        delete YACURS::Curses::statusline();

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
