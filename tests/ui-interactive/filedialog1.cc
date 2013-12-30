// $Id$
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "yacurs.h"

// Used when preloading libtestpreload.so

#ifdef YACURS_USE_WCHAR
extern "C" int
__test_wget_wch(void*, wint_t*) {
    throw std::runtime_error("Does not use preload lib");
    return ERR;
}
#else
extern "C" int
__test_wgetch(void*) {
    throw std::runtime_error("Does not use preload lib");
    return -1;
}
#endif

class MainWindow : public YACURS::Window {
    private:
        YACURS::VPack vpack1;
        YACURS::HPack hpack2;
        YACURS::VPack vpack21;
        YACURS::VPack vpack22;
        YACURS::HPack hpack1;
        YACURS::Button button1;
        YACURS::Button button2;
        YACURS::Button button3;
	YACURS::Button button4;
        YACURS::Label diagstatus;
        YACURS::Label file;
        YACURS::Label path;
        YACURS::Label filepath;
        YACURS::DynLabel label1;
        YACURS::DynLabel label2;
        YACURS::DynLabel label3;
        YACURS::DynLabel label4;
        YACURS::FileDialog* filedialogfile;
        YACURS::FileDialog* filedialogdirectory;
        YACURS::FileDialog* filedialogany;

    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt =
                dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (evt.data() == filedialogfile) {
                YACURS::Curses::statusbar()->push("FileDialogFile closed");

                if (filedialogfile->dialog_state() ==
                    YACURS::DIALOG_OK) {
                    label1.label("DIALOG_OK");
                    label2.label(filedialogfile->filepath() );
                    label3.label(filedialogfile->directory() );
                    label4.label(filedialogfile->filename() );
                } else {
                    label1.label("DIALOG_CANCEL");
                    label2.label("");
                    label3.label("");
                    label4.label("");
                }

                delete filedialogfile;
                filedialogfile = 0;
            }

            if (evt.data() == filedialogdirectory) {
                YACURS::Curses::statusbar()->push("FileDialogDirectory closed");

                if (filedialogdirectory->dialog_state() ==
                    YACURS::DIALOG_OK) {
                    label1.label("DIALOG_OK");
                    label2.label(filedialogdirectory->filepath() );
                    label3.label(filedialogdirectory->directory() );
                    label4.label(filedialogdirectory->filename() );
                } else {
                    label1.label("DIALOG_CANCEL");
                    label2.label("");
                    label3.label("");
                    label4.label("");
                }

                delete filedialogdirectory;
                filedialogdirectory = 0;
            }

            if (evt.data() == filedialogany) {
                YACURS::Curses::statusbar()->push("FileDialogAny closed");

                if (filedialogany->dialog_state() ==
                    YACURS::DIALOG_OK) {
                    label1.label("DIALOG_OK");
                    label2.label(filedialogany->filepath() );
                    label3.label(filedialogany->directory() );
                    label4.label(filedialogany->filename() );
                } else {
                    label1.label("DIALOG_CANCEL");
                    label2.label("");
                    label3.label("");
                    label4.label("");
                }

                delete filedialogany;
                filedialogany = 0;
            }
        }

        void button_press_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e =
                dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data() == &button1) {
                assert(filedialogfile == 0);

                filedialogfile = new YACURS::FileDialog("FileDialogFile");
		filedialogfile->selection_type(YACURS::FILE);
                filedialogfile->show();
                return;
            }

            if (e.data() == &button2) {
                assert(filedialogdirectory == 0);

                filedialogdirectory = new YACURS::FileDialog("FileDialogDirectory");
		filedialogdirectory->selection_type(YACURS::DIRECTORY);
                filedialogdirectory->show();
                return;
            }

            if (e.data() == &button3) {
                assert(filedialogany == 0);

                filedialogany = new YACURS::FileDialog("FileDialogAny");
                filedialogany->selection_type(YACURS::ANY);
                filedialogany->show();
                return;
            }

            if (e.data() == &button4) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow() : YACURS::Window(YACURS::Margin(1, 0, 1, 0) ),
		       button1("Dialog File"),
		       button2("Dialog Directory"),
		       button3("Dialog Any"),
		       button4("Quit"),
		       diagstatus("Dialog Status:"),
		       file("File:"),
		       path("Path:"),
		       filepath("File Path:"),
		       label1(),
		       label2(),
		       label3(),
		       label4(),
		       filedialogfile(0),
		       filedialogdirectory(0),
		       filedialogany(0) {
	    
            //vpack21->always_automatic(true);
            vpack22.always_dynamic(true);
            vpack22.hinting(false);
            hpack2.always_dynamic(true);
            hpack2.hinting(false);

            hpack2.add_back(&vpack21);
            hpack2.add_back(&vpack22);

            vpack21.add_back(&diagstatus);
            vpack21.add_back(&filepath);
            vpack21.add_back(&path);
            vpack21.add_back(&file);

            vpack22.add_front(&label4);
            vpack22.add_front(&label3);
            vpack22.add_front(&label2);
            vpack22.add_front(&label1);

            hpack1.add_back(&button1);
            hpack1.add_back(&button2);
            hpack1.add_back(&button3);
            hpack1.add_back(&button4);

            vpack1.add_back(&hpack2);
            vpack1.add_back(&hpack1);

            widget(&vpack1);

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
            if (filedialogfile)
                delete filedialogfile;
            if (filedialogdirectory)
                delete filedialogdirectory;
            if (filedialogany)
                delete filedialogany;

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
#ifdef ENABLE_NLS
    bindtextdomain("filedialog1", LOCALEDIR);
    textdomain("filedialog1");
#endif

    try {
        YACURS::Curses::init();

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   "FileDialog 1") );
        YACURS::Curses::statusbar(new YACURS::StatusBar);

        YACURS::Curses::mainwindow(new MainWindow);
        YACURS::Curses::mainwindow()->frame(true);

        YACURS::Curses::run();

        delete YACURS::Curses::mainwindow();
        delete YACURS::Curses::title();
        delete YACURS::Curses::statusbar();

        YACURS::Curses::end();
    } catch (std::runtime_error& e) {
	YACURS::Curses::end();
	std::cerr << e.what() << std::endl;
	return 77;
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
