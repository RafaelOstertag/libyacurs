// $Id$
//
// Test Pack
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include "yacurs.h"

// Used when preloading libtestpreload.so
int __test_data[] = {
    // First Input Widget
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // Jump to beginning, jump to end
    KEY_CTRL_A, KEY_CTRL_E,
    // jump to beginning, move curs forward
    KEY_CTRL_A, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    // move cursor backwards
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    // delete line
    KEY_CTRL_U,
    // enter line again
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // goto begin
    KEY_CTRL_A,
    // delete character
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    // delete line
    KEY_CTRL_A, KEY_CTRL_K,
    // Reenter line
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // Backspace delete
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    // Second Input Widget
    '\t',
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // Jump to beginning, jump to end
    KEY_CTRL_A, KEY_CTRL_E,
    // jump to beginning, move curs forward
    KEY_CTRL_A, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    // move cursor backwards
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    // delete line
    KEY_CTRL_U,
    // enter line again
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // goto begin
    KEY_CTRL_A,
    // delete character
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    // delete line
    KEY_CTRL_A, KEY_CTRL_K,
    // Reenter line
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l',
    'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't', ',', ' ', 'c', 'o',
    'n', 's', 'e', 'c', 't', 'e', 't', 'u', 'r', ' ', 'a', 'd', 'i', 'p', 'i',
    's', 'c', 'i', 'n', 'g', ' ', 'e', 'l', 'i', 't', '.', ' ', 'P', 'h', 'a',
    's', 'e', 'l', 'l', 'u', 's', ' ', 'v', 'e', 'n', 'e', 'n', 'a', 't', 'i',
    's', '.',
    // Backspace delete
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    // Read only Input widget
    '\t',
    // Hidden Input Widget
    '\t',
    // ListBox Widget
    '\t',
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
    KEY_CTRL_A, KEY_CTRL_E, KEY_CTRL_A,
    KEY_NPAGE, KEY_NPAGE, KEY_NPAGE, KEY_NPAGE, KEY_NPAGE,
    KEY_PPAGE, KEY_PPAGE, KEY_PPAGE, KEY_PPAGE, KEY_PPAGE,
    // Button 1
    '\t', '\n',

    // Buton 2
    '\t', '\n',

    // Button 3
    '\t',
    '\n', 0
};

extern "C" int
__test_wgetch(void*) {
    static int* ptr2 = __test_data;

    usleep(40000);

    if (*ptr2 == 0) {
        abort();
    }

    return *ptr2++;
}

// Event handler requires access
YACURS::Input<>* ifixed;
YACURS::Input<>* idyn;
YACURS::ListBox<>* listbox;
YACURS::Button* b1;
YACURS::Button* b2;
YACURS::Button* b3;
YACURS::Button* b4;

void
button_press_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_BUTTON_PRESS);

    YACURS::EventEx<YACURS::Button*>& ev =
        dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    if (ev.data() == b1) {
        b4->enabled(false);
        YACURS::Curses::statusline()->push_msg(ifixed->input() );
        return;
    }

    if (ev.data() == b2) {
        b4->enabled(true);
        YACURS::Curses::statusline()->push_msg(idyn->input() );
        return;
    }

    if (ev.data() == b3) {
        YACURS::EventQueue::submit(YACURS::EVT_QUIT);
        return;
    }

    if (ev.data() == b4) {
        assert(b4->enabled() );
    }

    return;
}

int
main() {
    std::list<std::string> items;

    for (int i = 0; i < 120; i++) {
        std::ostringstream n;
        n << i;
        items.push_back("Long Name ListBox Item Number " + n.str() );
    }

#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        YACURS::Curses::init();

        YACURS::EventQueue::connect_event(YACURS::EventConnectorFunction1(
                                              YACURS::EVT_BUTTON_PRESS,
                                              button_press_handler) );

        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                     POS_TOP,
                                                     "Focus 1") );
        YACURS::Curses::statusline(new YACURS::StatusLine);

        YACURS::Curses::mainwindow(new YACURS::Window(YACURS::Margin(1, 0, 1,
                                                                     0) ) );
        YACURS::Curses::mainwindow()->frame(true);

        YACURS::VPack* vpack = new YACURS::VPack;
        YACURS::HPack* hpack = new YACURS::HPack;
        b1 = new YACURS::Button("Button1");
        b2 = new YACURS::Button("Button2");
        b3 = new YACURS::Button("Quit");
        b4 = new YACURS::Button("Disabled");
        b4->enabled(false);
        ifixed = new YACURS::Input<>(10);
        idyn = new YACURS::Input<>;
        listbox = new YACURS::ListBox<>;
        listbox->set(items);
        YACURS::Input<>* ireadonly = new YACURS::Input<>;
        ireadonly->input("Read only");
        ireadonly->readonly(true);
        YACURS::Input<>* ihidden = new YACURS::Input<>;
        ihidden->input("Hidden Input");
        ihidden->hide_input(true);

        vpack->add_back(ifixed);
        vpack->add_back(idyn);
        vpack->add_back(ireadonly);
        vpack->add_back(ihidden);
        vpack->add_back(listbox);
        vpack->hinting(false);

        hpack->add_back(b1);
        hpack->add_back(b2);
        hpack->add_back(b3);
        hpack->add_back(b4);
        hpack->hinting(false);

        vpack->add_back(hpack);

        assert(b1->label() == "Button1");
        assert(b2->label() == "Button2");
        assert(b3->label() == "Quit");
        assert(b4->label() == "Disabled");

        YACURS::Curses::mainwindow()->widget(vpack);

        YACURS::Curses::run();

        delete b1;
        delete b2;
        delete b3;
        delete b4;
        delete idyn;
        delete ifixed;
        delete ireadonly;
        delete ihidden;
        delete listbox;
        delete hpack;
        delete vpack;
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
