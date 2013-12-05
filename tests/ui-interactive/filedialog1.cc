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
wint_t
#else
int
#endif
__test_data_1[] = {
    // Open dialog
    '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    // Up one dir
    KEY_DOWN, '\n',

    0
};

#ifdef YACURS_USE_WCHAR
wint_t
#else
int
#endif
__dir_up_data[] = {
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n',
    KEY_DOWN, '\n'
};

// Quit dialog
#ifdef YACURS_USE_WCHAR
wint_t
#else
int
#endif
__test_data_end_dialog[] = {
    '\t', '\t', '\t', '\t', '\n', 0
};

// Quit app
#ifdef YACURS_USE_WCHAR
wint_t
#else
int
#endif
__test_data_end_app[] = {
    '\t', '\t', '\n', 0
};

int* selection1[61];

int
select_item() {
    int tmp;

    while ( (tmp = (int)(drand48() * 100) % 20) < 2) ;

    return tmp;
}

void
quit() {
    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
}

#ifdef YACURS_USE_WCHAR
extern "C" int
__test_wget_wch(void*, wint_t* i) {
    static wint_t* ptr1 = __test_data_1;
    static wint_t* ptr_end_dialog = __test_data_end_dialog;
    static wint_t* ptr_end_app = __test_data_end_app;
    static int selection_index = 0;
    static int selection_index2 = 0;

    usleep(70000);

    // Open dialog
    if (*ptr1 != 0) {
	*i=*ptr1++;
	return OK;
    }

    if (selection1[selection_index] != 0) {
        if (selection1[selection_index][selection_index2] != 0) {
	    *i=selection1[selection_index][selection_index2++];
            return OK;
        } else {
            if (selection1[selection_index + 1] != 0) {
		*i = selection1[++selection_index][selection_index2 = 0];
                return OK;
	    }
        }
    }

    if (*ptr_end_dialog != 0) {
	*i = *ptr_end_dialog++;
        return OK;
    }

    if (*ptr_end_app != 0) {
	*i = *ptr_end_app++;
        return OK;
    }

    return ERR;
}
#else
extern "C" int
__test_wgetch(void*) {
    static int* ptr1 = __test_data_1;
    static int* ptr_end_dialog = __test_data_end_dialog;
    static int* ptr_end_app = __test_data_end_app;
    static int selection_index = 0;
    static int selection_index2 = 0;

    usleep(70000);

    // Open dialog
    if (*ptr1 != 0)
        return *ptr1++;

    if (selection1[selection_index] != 0) {
        if (selection1[selection_index][selection_index2] != 0) {
            return selection1[selection_index][selection_index2++];
        } else {
            if (selection1[selection_index + 1] != 0)
                return selection1[++selection_index][selection_index2 = 0];
        }
    }

    if (*ptr_end_dialog != 0)
        return *ptr_end_dialog++;

    if (*ptr_end_app != 0)
        return *ptr_end_app++;

    return 0;
}
#endif

class MainWindow : public YACURS::Window {
    private:
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack2;
        YACURS::VPack* vpack21;
        YACURS::VPack* vpack22;
        YACURS::HPack* hpack1;
        YACURS::Button* button1;
        YACURS::Button* button2;
        YACURS::Button* button3;
        YACURS::Label* diagstatus;
        YACURS::Label* file;
        YACURS::Label* path;
        YACURS::Label* filepath;
        YACURS::DynLabel* label1;
        YACURS::DynLabel* label2;
        YACURS::DynLabel* label3;
        YACURS::DynLabel* label4;
        YACURS::FileSaveDialog* filesavedialog;
        YACURS::FileLoadDialog* fileloaddialog;

    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt =
                dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (filesavedialog != 0 && evt.data() == filesavedialog) {
                YACURS::Curses::statusbar()->push("FileSaveDialog closed");

                if (filesavedialog->dialog_state() ==
                    YACURS::DIALOG_OK) {
                    label1->label("DIALOG_OK");
                    label2->label(filesavedialog->filepath() );
                    label3->label(filesavedialog->directory() );
                    label4->label(filesavedialog->filename() );
                } else {
                    label1->label("DIALOG_CANCEL");
                    label2->label("");
                    label3->label("");
                    label4->label("");
                }

                delete filesavedialog;
                filesavedialog = 0;
            }

            if (fileloaddialog != 0 && evt.data() == fileloaddialog) {
                YACURS::Curses::statusbar()->push("FileLoadDialog closed");

                if (fileloaddialog->dialog_state() ==
                    YACURS::DIALOG_OK) {
                    label1->label("DIALOG_OK");
                    label2->label(fileloaddialog->filepath() );
                    label3->label(fileloaddialog->directory() );
                    label4->label(fileloaddialog->filename() );
                } else {
                    label1->label("DIALOG_CANCEL");
                    label2->label("");
                    label3->label("");
                    label4->label("");
                }

                delete fileloaddialog;
                fileloaddialog = 0;
            }
        }

        void button_press_handler(YACURS::Event& _e) {
            assert(_e == YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e =
                dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data() == button1) {
                assert(filesavedialog == 0);

                filesavedialog = new YACURS::FileSaveDialog;
                filesavedialog->show();
                return;
            }

            if (e.data() == button3) {
                assert(fileloaddialog == 0);

                fileloaddialog = new YACURS::FileLoadDialog;
                fileloaddialog->show();
                return;
            }

            if (e.data() == button2) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow() : YACURS::Window(YACURS::Margin(1, 0, 1, 0) ),
            filesavedialog(0),
            fileloaddialog(0) {
            button1 = new YACURS::Button("File Save");
            button3 = new YACURS::Button("File Load");
            button2 = new YACURS::Button("Quit");
            vpack1 = new YACURS::VPack;
            hpack1 = new YACURS::HPack;
            label1 = new YACURS::DynLabel;
            label2 = new YACURS::DynLabel;
            label3 = new YACURS::DynLabel;
            label4 = new YACURS::DynLabel;

            diagstatus = new YACURS::Label("Dialog Status:");
            file = new YACURS::Label("File:");
            path = new YACURS::Label("Path:");
            filepath = new YACURS::Label("File Path:");

            hpack2 = new YACURS::HPack;
            vpack21 = new YACURS::VPack;
            vpack22 = new YACURS::VPack;

            //vpack21->always_automatic(true);
            vpack22->always_dynamic(true);
            vpack22->hinting(false);
            hpack2->always_dynamic(true);
            hpack2->hinting(false);

            hpack2->add_back(vpack21);
            hpack2->add_back(vpack22);

            vpack21->add_back(diagstatus);
            vpack21->add_back(filepath);
            vpack21->add_back(path);
            vpack21->add_back(file);

            vpack22->add_front(label4);
            vpack22->add_front(label3);
            vpack22->add_front(label2);
            vpack22->add_front(label1);

            hpack1->add_back(button1);
            hpack1->add_back(button3);
            hpack1->add_back(button2);

            vpack1->add_back(hpack2);
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
            if (filesavedialog)
                delete filesavedialog;
            if (fileloaddialog)
                delete fileloaddialog;

            delete button1;
            delete button2;
            delete button3;
            delete hpack1;
            delete vpack1;
            delete label1;
            delete label2;
            delete label3;
            delete label4;
            delete hpack2;
            delete vpack21;
            delete vpack22;
            delete diagstatus;
            delete file;
            delete path;
            delete filepath;

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

    srand48(time(0) );

    // Initialize test data
    for (int i = 0; i < 30; i++) {
        if (i % 2 == 0) {
            int tmp = select_item();
            selection1[i] = (int*)calloc(tmp + 2, sizeof (int) );

            for (int n = 0; n < tmp; n++)
                selection1[i][n] = KEY_DOWN;

            selection1[i][tmp] = '\n';
            selection1[i][tmp + 1] = 0;
        } else {
            selection1[i] = (int*)calloc(21, sizeof (int) );
            std::memcpy(selection1[i], __dir_up_data, 20 * sizeof (int) );
            selection1[i][20] = 0;
        }
    }

    selection1[60] = 0;

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
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    for (int i = 0; i < 30; i++)
        std::free(selection1[i]);

    return 0;
}
