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
__test_data[10][5] = {
    // OK_ONLY
    { '\n', '\n', 0 },
    // YES_ONLY
    { '\t', '\n', '\n', 0 },
    // OKCANCEL: OK
    { '\t', '\n', '\n', 0 },
    // OKCANCEL: Cancel
    { '\n', '\t', '\n', 0 },
    // YESNO: Yes
    { '\t', '\n', '\n', 0 },
    // YESNO: No
    { '\n', '\t', '\n', 0 },
    // YESNOCANCEL: Yes
    { '\t', '\n', '\n', 0 },
    // YESNOCANCEL: No
    { '\n', '\t', '\n', 0 },
    // YESNOCANCEL: Cancel
    { '\n', '\t', '\t', '\n', 0 },
    // QUIT
    { '\t', '\n', 0 }
};

YACURS::DIALOG_STATE expectation[9] = {
    YACURS::DIALOG_OK,
    YACURS::DIALOG_YES,
    YACURS::DIALOG_OK,
    YACURS::DIALOG_CANCEL,
    YACURS::DIALOG_YES,
    YACURS::DIALOG_NO,
    YACURS::DIALOG_YES,
    YACURS::DIALOG_NO,
    YACURS::DIALOG_CANCEL
};


static YACURS::DIALOG_STATE last_dialog_state;

#ifdef YACURS_USE_WCHAR
extern "C" int
__test_wget_wch(void*, wint_t* i) {
    static int row = 0;
    static int col = 0;
    static YACURS::DIALOG_STATE* exptr = expectation;

    usleep(70000);

    if (__test_data[row][col] == 0) {
	// Test expectation
	if (*exptr != last_dialog_state)
	    abort();

	// Next round
	exptr++;
	if (++row>9)
	    abort();

	col=0;
    }

    *i = __test_data[row][col++];

    return OK;
}
#else
extern "C" int
__test_wgetch(void*) {
    static int row = 0;
    static int col = 0;
    static YACURS::DIALOG_STATE* exptr = expectation;

    usleep(70000);

    if (__test_data[row][col] == 0) {
	// Test expectation
	if (*exptr != last_dialog_state)
	    abort();

	// Next round
	exptr++;
	if (++row>9)
	    abort();

	col=0;
    }

    return __test_data[row][col++];
}
#endif

class MainWindow : public YACURS::Window {
    private:
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack1;
        YACURS::Button* button1;
        YACURS::Button* button2;
        YACURS::Button* button3;
        YACURS::Button* button4;
        YACURS::Button* button5;
        YACURS::Button* button6;
        YACURS::Label* label1;
	YACURS::Dialog* dialog;

    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt =
                dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (dialog != 0 && evt.data() == dialog) {
#ifdef YACURS_USE_WCHAR
		YACURS::Curses::statusbar()->push("Dialog 1 clösed");
#else
                YACURS::Curses::statusbar()->push("Dialog 1 closed");
#endif

		switch (last_dialog_state=dialog->dialog_state()) {
		case YACURS::DIALOG_OK:
                    label1->label("DIALOG_OK");
		    break;
		case YACURS::DIALOG_YES:
		    label1->label("DIALOG_YES");
		    break;
		case YACURS::DIALOG_CANCEL:
		    label1->label("DIALOG_CANCEL");
		    break;
		case YACURS::DIALOG_NO:
		    label1->label("DIALOG_NO");
		    break;
		}

                delete dialog;
                dialog = 0;
            }
        }

        void button_press_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e =
                dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data() == button1) {
                assert(dialog == 0);

                dialog = new YACURS::Dialog("OK_ONLY Dialog", YACURS::OK_ONLY);
                dialog->show();
                return;
            }

            if (e.data() == button2) {
                assert(dialog == 0);

                dialog = new YACURS::Dialog("YES_ONLY Dialog", YACURS::YES_ONLY);
                dialog->show();
                return;
            }

            if (e.data() == button3) {
                assert(dialog == 0);

                dialog = new YACURS::Dialog("OKCANCEL Dialog", YACURS::OKCANCEL);
                dialog->show();
                return;
            }

            if (e.data() == button4) {
                assert(dialog == 0);

                dialog = new YACURS::Dialog("YESNO Dialog", YACURS::YESNO);
                dialog->show();
                return;
            }

            if (e.data() == button5) {
                assert(dialog == 0);

                dialog = new YACURS::Dialog("YESNOCANCEL Dialog", YACURS::YESNOCANCEL);
                dialog->show();
                return;
            }

            if (e.data() == button6) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow() : YACURS::Window(YACURS::Margin(1, 0, 1,
                                                     0) ),
		       dialog(0) {
            button1 = new YACURS::Button("OK_ONLY");
            button2 = new YACURS::Button("YES_ONLY");
            button3 = new YACURS::Button("OKCANCEL");
            button4 = new YACURS::Button("YESNO");
            button5 = new YACURS::Button("YESNOCANCEL");
            button6 = new YACURS::Button("Quit");
            vpack1 = new YACURS::VPack;
            hpack1 = new YACURS::HPack;
            label1 = new YACURS::Label("dialog state");
            hpack1->add_back(button1);
            hpack1->add_back(button2);
            hpack1->add_back(button3);
            hpack1->add_back(button4);
            hpack1->add_back(button5);
            hpack1->add_back(button6);
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
            if (dialog)
                delete dialog;

            delete button1;
            delete button2;
            delete button3;
            delete button4;
            delete button5;
            delete button6;
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
    if (setlocale(LC_ALL,"en_US.UTF-8")==0) exit(77);
#endif

    try {
        YACURS::Curses::init();

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   "Dialog 1") );
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
