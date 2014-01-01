// $Id$
//
// Test application
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>

#include "yacurs.h"

// Used when preloading libtestpreload.so
#ifdef YACURS_USE_WCHAR
wint_t
#else
int
#endif
__test_data[] = {
    // Open dialog, and cancel
    '\n', '\t', '\n',
    // Open Dialog, and Ok
    '\n', '\n',
    // Quit app
    '\t', '\n', 0
};

#ifdef YACURS_USE_WCHAR
extern "C" int
__test_wget_wch(void*, wint_t* i) {
    static wint_t* ptr2 = __test_data;

    #ifdef SLOW_TESTS
	usleep(70000);
#endif

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

    #ifdef SLOW_TESTS
	usleep(70000);
#endif

    if (*ptr2 == 0) {
        abort();
    }

    return *ptr2++;
}
#endif

class MainWindow : public YACURS::Window {
    private:
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack1;
        YACURS::Button* button1;
        YACURS::Button* button2;
        YACURS::Label* label1;
        YACURS::MessageBox* messagebox1;

    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt =
                dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (messagebox1 != 0 && evt.data() == messagebox1) {
                YACURS::Curses::statusbar()->push("MessageBox closed");

                if (messagebox1->dialog_state() == YACURS::DIALOG_OK)
                    label1->label("DIALOG_OK");
                else
                    label1->label("DIALOG_CANCEL");

                delete messagebox1;
                messagebox1 = 0;
            }
        }

        void button_press_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e =
                dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data() == button1) {
                assert(messagebox1 == 0);

#ifdef YACURS_USE_WCHAR
                messagebox1 = new YACURS::MessageBox("«Test MessageBox»",
                                                     "“This is the message”");
#else
                messagebox1 = new YACURS::MessageBox("Test MessageBox",
                                                     "This is the message");
#endif
                messagebox1->show();
                return;
            }

            if (e.data() == button2) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow() : YACURS::Window(YACURS::Margin(1, 0, 1,
                                                     0) ), messagebox1(0) {
#ifdef YACURS_USE_WCHAR
            button1 = new YACURS::Button("«New Window»");
            button2 = new YACURS::Button("«Quit»");
#else
            button1 = new YACURS::Button("New Window");
            button2 = new YACURS::Button("Quit");
#endif
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
            if (messagebox1)
                delete messagebox1;

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
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO)!=1 ||
	isatty(STDIN_FILENO)!=1) exit(77);

#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef YACURS_USE_WCHAR
    setlocale(LC_ALL,"");
#endif

    try {
        YACURS::Curses::init();

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   "YACURS::MessageBox") );
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
