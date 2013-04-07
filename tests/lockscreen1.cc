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

#define KEY_SLEEP -1978

// Used when preloading libtestpreload.so
int __test_data[]= {
    // Press first button
    '\n',
    // enter text
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    '\t', '\n',
    // next button
    '\t', '\n',

    // get off the list box, and select Fill button
    '\t', '\t',
    // press button
    '\n',
    // Select Clear button,
    KEY_LEFT, '\n',
    // Select Fill button
    KEY_RIGHT, '\n',
    // Select Clear button,
    KEY_LEFT, '\n',
    // Select Fill button
    KEY_RIGHT, '\n',
    // Select Clear button,
    KEY_LEFT, '\n',
    // Select Fill button
    KEY_RIGHT, '\n',

    // close window
    '\t', '\n',

    // open box dialog
    KEY_RIGHT, '\n',

    // Select like crazy
    ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN,

    '\t',
    // Radio Box
    ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN, ' ', KEY_DOWN,

    // Check Box again
    '\t', '\t',
    ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP,

    // Radio Box again
    '\t',
    ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP, ' ', KEY_UP,

    // Select OK button
    '\t', '\n',

    // Advance to the first button
    '\t', '\t',

    // Let Screen Lock kick in
    KEY_SLEEP,

    // Quit
    '\t', '\n', 0
};

int __unlock_screen[] = {
    // Press any key to activate unlock dialog
    'a',
    // Enter Secret
    'U', 'n', 'l', 'o', 'c', 'k', ' ', 'S', 'c', 'r', 'e', 'e', 'n',
    '\n', '\n',
    0
};

int __quit_data[] = {
    KEY_LEFT, '\n', 0
};

extern "C" int __test_wgetch(void*) {
    static int* ptr2=__test_data;
    static int rounds=0;

    usleep(100);

    if (*ptr2==0) {
        ptr2=__test_data;
    }

    if (*ptr2==KEY_SLEEP) {
        if (rounds>10) {
            ptr2=__quit_data;
        } else {
            sleep(7);
            ptr2=__unlock_screen;
            rounds++;
            return ERR;
        }
    }

    return *ptr2++;
}



class Win1: public YACURS::Window {
    private:
        YACURS::Label* l1;
        YACURS::Input<>* i1;
        YACURS::Button* close1;
        YACURS::VPack* vp1;

    protected:
        void button_press_handler(YACURS::Event& _e) {
            assert(_e==YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e=dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data()==close1) {
                close();
            }

            return;
        }

    public:
        Win1(): YACURS::Window(YACURS::Margin(3,2,3,2)) {
            frame(true);
            l1 = new YACURS::Label("Enter Text");
            i1 = new YACURS::Input<>;
            close1 = new YACURS::Button("Close");
            vp1 = new YACURS::VPack;
            vp1->add_back(l1);
            vp1->add_back(i1);
            vp1->add_back(close1);
            widget(vp1);

            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<Win1>(YACURS::EVT_BUTTON_PRESS, this, &Win1::button_press_handler));
        }

        ~Win1() {
            delete l1;
            delete i1;
            delete close1;
            delete vp1;

            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<Win1>(YACURS::EVT_BUTTON_PRESS, this, &Win1::button_press_handler));
        }
};

class ListBoxWin: public YACURS::Window {
    private:
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack1;
        YACURS::ListBox<>* listbox;
        YACURS::Button* bclear;
        YACURS::Button* badd;
        YACURS::Button* bclose;

    protected:
        void button_press_handler(YACURS::Event& _e) {
            assert(_e==YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e=dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data()==bclear) {
                listbox->clear();
                return;
            }

            if (e.data()==badd) {
                for (int i=0; i<120; i++) {
                    std::ostringstream n;
                    n<<i;
                    listbox->add("Long Name ListBox Item Number " + n.str());
                }

                return;
            }

            if (e.data()==bclose) {
                close();
            }

            return;
        }

    public:
        ListBoxWin(): YACURS::Window(YACURS::Margin(3,2,3,2)) {
            frame(true);
            listbox=new YACURS::ListBox<>;
            bclear=new YACURS::Button("Clear");
            badd=new YACURS::Button("Fill");
            bclose=new YACURS::Button("Close");
            vpack1=new YACURS::VPack;
            hpack1=new YACURS::HPack;

            hpack1->add_back(bclear);
            hpack1->add_back(badd);
            hpack1->add_back(bclose);
            vpack1->add_back(listbox);
            vpack1->add_back(hpack1);

            widget(vpack1);

            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<ListBoxWin>(YACURS::EVT_BUTTON_PRESS, this, &ListBoxWin::button_press_handler));
        }

        ~ListBoxWin() {
            delete listbox;
            delete bclear;
            delete badd;
            delete bclose;
            delete vpack1;
            delete hpack1;

            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<ListBoxWin>(YACURS::EVT_BUTTON_PRESS, this, &ListBoxWin::button_press_handler));
        }
};

class BoxDialog: public YACURS::Dialog {
    private:
        YACURS::HPack* hpack;
        YACURS::CheckBox* checkbox;
        YACURS::RadioBox* radiobox;

    public:
        BoxDialog(): YACURS::Dialog("Box Dialog", YACURS::Dialog::OK_ONLY),
            hpack(0),
            checkbox(0),
            radiobox(0) {
            hpack=new YACURS::HPack;
            std::vector<std::string> items;

            for (int i=0; i<10; i++) {
                std::ostringstream _i;
                _i<<i;
                items.push_back("Check Box Item " + _i.str());
            }

            checkbox=new YACURS::CheckBox("", items);

            items.clear();

            for (int i=0; i<10; i++) {
                std::ostringstream _i;
                _i<<i;
                items.push_back("Radio Box Item " + _i.str());
            }

            radiobox=new YACURS::RadioBox("", items);

            hpack->add_back(checkbox);
            hpack->add_back(radiobox);
            widget(hpack);
        }

        ~BoxDialog() {
            assert(hpack!=0);
            assert(checkbox!=0);
            assert(radiobox!=0);
            delete hpack;
            delete checkbox;
            delete radiobox;
        }
};

class MainWindow: public YACURS::Window {
    private:
        YACURS::HPack* hpack1;
        YACURS::Button* button1;
        YACURS::Button* button2;
        YACURS::Button* button3;
        YACURS::Button* button4;
        Win1* win1;
        ListBoxWin* lbwin;
        BoxDialog* boxdialog;


    protected:
        void window_close_handler(YACURS::Event& _e) {
            assert(_e==YACURS::EVT_WINDOW_CLOSE);
            YACURS::EventEx<YACURS::WindowBase*>& evt=dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (win1!=0 && evt.data()==win1) {
                YACURS::Curses::statusline()->push_msg("Window 1 closed");
                delete win1;
                win1=0;
                return;
            }

            if (lbwin!=0 && evt.data()==lbwin) {
                delete lbwin;
                lbwin=0;
                return;
            }

            if (boxdialog!=0 && evt.data()==boxdialog) {
                delete boxdialog;
                boxdialog=0;
                return;
            }
        }

        void button_press_handler(YACURS::Event& _e) {
            assert(_e==YACURS::EVT_BUTTON_PRESS);
            YACURS::EventEx<YACURS::Button*>& e=dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (e.data()==button1) {
                assert(win1==0);

                win1=new Win1;
                win1->show();
                return;
            }

            if (e.data()==button3) {
                assert(lbwin==0);

                lbwin=new ListBoxWin;
                lbwin->show();
                return;
            }

            if (e.data()==button2) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }

            if (e.data()==button4) {
                assert(boxdialog==0);
                boxdialog=new BoxDialog;
                boxdialog->show();
            }
        }

    public:
        MainWindow(): YACURS::Window(YACURS::Margin(1,0,1,0)), win1(0), lbwin(0), boxdialog(0) {
            button1=new YACURS::Button("New Window");
            button2=new YACURS::Button("Quit");
            button3=new YACURS::Button("List Box Win");
            button4=new YACURS::Button("Box Dialog");
            hpack1=new YACURS::HPack();
            hpack1->add_back(button1);
            hpack1->add_back(button3);
            hpack1->add_back(button4);
            hpack1->add_back(button2);
            widget(hpack1);

            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
        }

        ~MainWindow() {
            if (win1)
                delete win1;

            if (lbwin)
                delete lbwin;

            delete button1;
            delete button2;
            delete button3;
            delete button4;
            delete hpack1;

            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
        }
};

int main() {
    YACURS::LockScreen* lckscr;
    YACURS::UnlockDialogDefault* ulckdia;
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        YACURS::Curses::init();

        ulckdia=new YACURS::UnlockDialogDefault("Unlock Screen");
        lckscr=new YACURS::LockScreen(ulckdia);

        YACURS::EventQueue::lock_screen(lckscr);
        YACURS::EventQueue::timeout(5);

        YACURS::Curses::title(new YACURS::LineObject(YACURS::LineObject::POS_TOP,
                                     "LockScreen 1"));
        YACURS::Curses::statusline(new YACURS::StatusLine);

        MainWindow* mainwindow=new MainWindow;
        YACURS::Curses::mainwindow(mainwindow);
        YACURS::Curses::mainwindow()->frame(true);

        YACURS::Curses::run();

        delete mainwindow;
        delete ulckdia;
        delete lckscr;
        delete YACURS::Curses::statusline();
        delete YACURS::Curses::title();

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
