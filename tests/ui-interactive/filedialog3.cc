// $Id: filedialog2.cc 5740 2013-12-08 11:28:03Z rafisol $
//
// same test a filedialog2.cc, but using suffix()
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <unistd.h>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "yacurs.h"

void quit() { YACURS::EventQueue::submit(YACURS::EVT_QUIT); }

#ifdef YACURS_USE_WCHAR
extern "C" int __test_wget_wch(void*, wint_t*) {
    throw std::runtime_error("Does not use preload lib");
    return ERR;
}
#else
extern "C" int __test_wgetch(void*) {
    throw std::runtime_error("Does not use preload lib");
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

            if (filesavedialog->dialog_state() == YACURS::DIALOG_OK) {
                label1->label("DIALOG_OK");
                label2->label(filesavedialog->filepath());
                label3->label(filesavedialog->directory());
                label4->label(filesavedialog->filename());
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

            if (fileloaddialog->dialog_state() == YACURS::DIALOG_OK) {
                label1->label("DIALOG_OK");
                label2->label(fileloaddialog->filepath());
                label3->label(fileloaddialog->directory());
                label4->label(fileloaddialog->filename());
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

            filesavedialog = new YACURS::FileSaveDialog(std::string(), true);
            filesavedialog->suffix(".suffix");
            filesavedialog->show();
            return;
        }

        if (e.data() == button3) {
            assert(fileloaddialog == 0);

            fileloaddialog = new YACURS::FileLoadDialog(std::string(), true);
            fileloaddialog->suffix(".suffix");
            fileloaddialog->show();
            return;
        }

        if (e.data() == button2) {
            YACURS::EventQueue::submit(YACURS::EVT_QUIT);
            return;
        }
    }

   public:
    MainWindow()
        : YACURS::Window(YACURS::Margin(1, 0, 1, 0)),
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

        // vpack21->always_automatic(true);
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

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<MainWindow>(
                YACURS::EVT_BUTTON_PRESS, this,
                &MainWindow::button_press_handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<MainWindow>(
                YACURS::EVT_WINDOW_CLOSE, this,
                &MainWindow::window_close_handler));
    }

    ~MainWindow() {
        if (filesavedialog) delete filesavedialog;
        if (fileloaddialog) delete fileloaddialog;

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

        YACURS::EventQueue::disconnect_event(
            YACURS::EventConnectorMethod1<MainWindow>(
                YACURS::EVT_BUTTON_PRESS, this,
                &MainWindow::button_press_handler));
        YACURS::EventQueue::disconnect_event(
            YACURS::EventConnectorMethod1<MainWindow>(
                YACURS::EVT_WINDOW_CLOSE, this,
                &MainWindow::window_close_handler));
    }
};

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL, "en_US.UTF-8") == 0) exit(77);
#endif
#ifdef ENABLE_NLS
    bindtextdomain("filedialog2", LOCALEDIR);
    textdomain("filedialog2");
#endif

    try {
        YACURS::Curses::init();

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::POS_TOP,
                                                   "FileDialog 3 (w/ suffix)"));
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
