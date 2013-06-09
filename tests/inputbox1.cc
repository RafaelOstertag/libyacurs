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
#define C_L L'ł'
#define C_O L'ø'
#define C_R L'®'
#define C_E L'€'
#define C_M L'µ'
#define C_I L'ı'
#define C_P L'Þ'
#define C_S L'ß'
#define C_U L'↓'
wint_t
#else
#define C_L 'l'
#define C_O 'o'
#define C_R 'r'
#define C_E 'e'
#define C_M 'm'
#define C_I 'i'
#define C_P 'p'
#define C_S 's'
#define C_U 'u'
int
#endif
__test_data[] = {
    // Open dialog
    '\n',
    // enter text
    C_L, C_O, C_R, C_E, C_M, ' ', C_I, C_P, C_S, C_U, C_M, ' ', 'd', C_O, C_L,
    C_O, C_R, ' ', C_S, C_I, 't', ' ', 'a', C_M, C_E, 't', ',', ' ', 'c', C_O,
    'n', C_S, C_E, 'c', 't', C_E, 't', C_U, C_R, ' ', 'a', 'd', C_I, C_P, C_I,
    C_S, 'c', C_I, 'n', 'g', ' ', C_E, C_L, C_I, 't', '.', ' ', 'P', 'h', 'a',
    C_S, C_E, C_L, C_L, C_U, C_S, ' ', 'v', C_E, 'n', C_E, 'n', 'a', 't', C_I,
    C_S, '.',
    // Ok, dialog
    '\t', '\n',
    // Open Dialog
    '\n',
    // enter text
    C_L, C_O, C_R, C_E, C_M, ' ', C_I, C_P, C_S, C_U, C_M, ' ', 'd', C_O, C_L,
    C_O, C_R, ' ', C_S, C_I, 't', ' ', 'a', C_M, C_E, 't', ',', ' ', 'c', C_O,
    'n', C_S, C_E, 'c', 't', C_E, 't', C_U, C_R, ' ', 'a', 'd', C_I, C_P, C_I,
    C_S, 'c', C_I, 'n', 'g', ' ', C_E, C_L, C_I, 't', '.', ' ', 'P', 'h', 'a',
    C_S, C_E, C_L, C_L, C_U, C_S, ' ', 'v', C_E, 'n', C_E, 'n', 'a', 't', C_I,
    C_S, '.',
    // cancel dialog
    '\t', '\t', '\n',
    // Quit app
    '\t', '\n', 0
};
#undef C_L
#undef C_O
#undef C_R
#undef C_E
#undef C_M
#undef C_I
#undef C_P
#undef C_S
#undef C_U

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

    usleep(70000);

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
        YACURS::InputBox* inputbox1;

    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt =
                dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (inputbox1 != 0 && evt.data() == inputbox1) {
                YACURS::Curses::statusbar()->push_msg("Dialog 1 closed");

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

#ifdef USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==NULL) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   "YACURS::InputBox 1") );
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
