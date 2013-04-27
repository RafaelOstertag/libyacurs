// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_TERMIOS_H
# include <termios.h>
#else // HAVE_TERMIOS_H
# ifdef HAVE_SYS_TERMIOS_H
#  include <sys/termios.h>
# endif // HAVE_SYS_TERMIOS_H
#endif // HAVE_TERMIOS_H

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "yacurs.h"

void
alrm(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_SIGALRM);

    std::string status_msg("Size: rows=");

    YACURS::Size _scrdim(YACURS::Curses::inquiry_screensize() );

    char buff[32];
    snprintf(buff, 32, "%d", _scrdim.rows() );
    status_msg += buff;

    status_msg += " cols=";

    snprintf(buff, 32, "%d", _scrdim.cols() );
    status_msg += buff;

    YACURS::Curses::statusbar()->push_msg(status_msg);

    winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        return;
    }

    ws.ws_row--;
    ws.ws_col -= 4;

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
        return;
    }

    if (ws.ws_row <= YACURS::MIN_ROWS ||
        ws.ws_col <= YACURS::MIN_COLS)
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
    else
        alarm(1);
}

int
main() {
    winsize wsave;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wsave) == -1) {
        return 1;
    }

    try {
        YACURS::Curses::init();

        YACURS::TitleBar* title = new YACURS::TitleBar(
            YACURS::TitleBar::POS_TOP,
            "Widget Resize 1");
        YACURS::Curses::title(title);

        YACURS::StatusBar* sl = new YACURS::StatusBar();
        YACURS::Curses::statusbar(sl);

        YACURS::Window* w1 = new YACURS::Window(YACURS::Margin(1, 0, 1, 0) );
        w1->frame(true);

        YACURS::VPack* vpack = new YACURS::VPack;
        YACURS::HPack* hpack = new YACURS::HPack;
        YACURS::HPack* hpack1 = new YACURS::HPack();
        YACURS::HPack* hpack2 = new YACURS::HPack();
        YACURS::VPack* vpack1 = new YACURS::VPack;
        YACURS::VPack* vpack2 = new YACURS::VPack;
        YACURS::VPack* vpack2_1 = new YACURS::VPack();
        YACURS::VPack* vpack2_2 = new YACURS::VPack();

        YACURS::Label* label1 = new YACURS::Label("Test label");
        YACURS::Label* label2 = new YACURS::Label("Test label2");
        YACURS::Label* label3 = new YACURS::Label("Test label3");
        YACURS::Label* label4 = new YACURS::Label("Test label4");
        YACURS::Label* label5 = new YACURS::Label("Label 5");
        YACURS::Label* label6 = new YACURS::Label("Label 6");
        YACURS::Label* label7 = new YACURS::Label("Label 7");
        YACURS::Label* label8 = new YACURS::Label("Label 8");
        YACURS::Label* label9 = new YACURS::Label("Label 9");
        YACURS::Label* label10 = new YACURS::Label("Label 10");
        YACURS::Label* label11 = new YACURS::Label("Label 11");
        YACURS::Label* label12 = new YACURS::Label("Label 12");
        YACURS::Label* label13 = new YACURS::Label("Label 13");
        YACURS::Label* label14 = new YACURS::Label("Label 14");
        YACURS::Label* label15 = new YACURS::Label("Label 15");
        YACURS::Label* label16 = new YACURS::Label("Label 16");

        vpack->add_front(hpack);
        vpack->add_front(hpack1);
        vpack->add_back(hpack2);

        hpack->always_dynamic(true);
        hpack->hinting(true);
        hpack->add_back(vpack1);
        hpack->add_back(vpack2);

        hpack1->always_dynamic(true);
        hpack1->hinting(true);
        hpack1->add_back(vpack2_1);
        hpack1->add_back(vpack2_2);

        vpack1->always_dynamic(true);
        vpack1->hinting(false);
        vpack1->add_back(label1);
        vpack1->add_back(label2);
        vpack1->add_front(label3);

        vpack2->always_dynamic(true);
        vpack2->hinting(false);
        vpack2->add_back(label4);
        vpack2->add_back(label5);
        vpack2->add_back(label6);

        vpack2_1->always_dynamic(true);
        vpack2_1->hinting(false);
        vpack2_1->add_front(label7);
        vpack2_1->add_front(label8);
        vpack2_1->add_front(label9);

        vpack2_2->always_dynamic(true);
        vpack2_2->hinting(false);
        vpack2_2->add_back(label10);
        vpack2_2->add_back(label11);
        vpack2_2->add_back(label12);
        vpack2_2->add_back(label13);
        vpack2_2->add_back(label14);

        hpack2->always_dynamic(true);
        hpack2->hinting(true);
        hpack2->add_back(label15);
        hpack2->add_back(label16);

        label1->label("New Test Label");
        label2->label("New Test Label2");
        label3->label("New Test Label3");
        label4->label("New Test Label4");

        w1->widget(vpack);

        YACURS::Curses::mainwindow(w1);

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_SIGALRM, &alrm) );

        alarm(3);
        YACURS::Curses::run();

        delete title;

        delete label1;
        delete label2;
        delete label3;
        delete label4;
        delete label5;
        delete label6;
        delete label7;
        delete label8;
        delete label9;
        delete label10;
        delete label11;
        delete label12;
        delete label13;
        delete label14;
        delete label15;
        delete label16;
        delete vpack1;
        delete vpack2;
        delete vpack2_1;
        delete vpack2_2;
        delete hpack1;
        delete hpack2;
        delete vpack;
        delete hpack;

        delete w1;
        delete sl;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();

        if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
            return 1;
        }

        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
        return 1;
    }

    return 0;
}
