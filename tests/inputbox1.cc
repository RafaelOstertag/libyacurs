// $Id: messagebox1.cc 4865 2013-02-18 18:43:42Z rafisol $
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
int __test_data[]= {
    // Open dialog
    '\n',
    // enter text
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // Ok, dialog
    '\t', '\n',
    // Open Dialog
    '\n',
    // enter text
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // cancel dialog
    '\t', '\t', '\n',
    // Quit app
    '\t', '\n', 0
};

extern "C" int __test_wgetch(void*) {
    static int* ptr2=__test_data;

    usleep(70000);

    if (*ptr2==0) {
        abort();
    }

    return *ptr2++;
}

class MainWindow: public Window {
    private:
        VPack* vpack1;
        HPack* hpack1;
        Button* button1;
        Button* button2;
        Label* label1;
        InputBox* inputbox1;


    protected:
        void window_close_handler(Event& _e) {
            assert(_e==EVT_WINDOW_CLOSE);
            EventEx<WindowBase*>& evt=dynamic_cast<EventEx<WindowBase*>&>(_e);

            if (inputbox1!=0 && evt.data()==inputbox1) {
                Curses::statusline()->push_msg("Dialog 1 closed");

                if (inputbox1->dialog_state()==Dialog::DIALOG_OK)
                    label1->label("DIALOG_OK");
                else
                    label1->label("DIALOG_CANCEL");

                delete inputbox1;
                inputbox1=0;
            }

        }

        void button_press_handler(Event& _e) {
            assert(_e==EVT_BUTTON_PRESS);
            EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

            if (e.data()==button1) {
                assert(inputbox1==0);

                inputbox1=new InputBox("Test Dialog", "Enter text");
                inputbox1->show();
                return;
            }

            if (e.data()==button2) {
                EventQueue::submit(EVT_QUIT);
                return;
            }
        }

    public:
        MainWindow(): Window(Margin(1,0,1,0)), inputbox1(0) {
            button1=new Button("New Window");
            button2=new Button("Quit");
            vpack1=new VPack;
            hpack1=new HPack;
            label1=new Label("dialog state");
            hpack1->add_back(button1);
            hpack1->add_back(button2);
            vpack1->add_front(label1);
            vpack1->add_back(hpack1);
            widget(vpack1);

            EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
            EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
        }

        ~MainWindow() {
            if (inputbox1)
                delete inputbox1;

            delete button1;
            delete button2;
            delete hpack1;
            delete vpack1;
            delete label1;

            EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
            EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
        }
};

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        Curses::init();

        Curses::title(new LineObject(LineObject::POS_TOP,
                                     "InputBox 1"));
        Curses::statusline(new StatusLine);

        MainWindow* mainwindow=new MainWindow;
        Curses::mainwindow(mainwindow);
        Curses::mainwindow()->frame(true);

        Curses::run();

        delete mainwindow;

        Curses::end();
    } catch (std::exception& e) {
        Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
