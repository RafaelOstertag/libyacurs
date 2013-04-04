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
int __test_data[]= {
    // First Input Widget
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // Jump to beginning, jump to end
    KEY_CTRL_A, KEY_CTRL_E,
    // jump to beginning, move curs forward
    KEY_CTRL_A, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    // move cursor backwards
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    // delete line
    KEY_CTRL_U,
    // enter line again
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // goto begin
    KEY_CTRL_A,
    // delete character
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    // delete line
    KEY_CTRL_A, KEY_CTRL_K,
    // Reenter line
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // Backspace delete
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    // Second Input Widget
    '\t',
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // Jump to beginning, jump to end
    KEY_CTRL_A, KEY_CTRL_E,
    // jump to beginning, move curs forward
    KEY_CTRL_A, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F, KEY_CTRL_F,
    // move cursor backwards
    KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B, KEY_CTRL_B,
    // delete line
    KEY_CTRL_U,
    // enter line again
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // goto begin
    KEY_CTRL_A,
    // delete character
    KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D, KEY_CTRL_D,
    // delete line
    KEY_CTRL_A, KEY_CTRL_K,
    // Reenter line
    'L','o','r','e','m',' ','i','p','s','u','m',' ','d','o','l','o','r',' ','s','i','t',' ','a','m','e','t',',',' ','c','o','n','s','e','c','t','e','t','u','r',' ','a','d','i','p','i','s','c','i','n','g',' ','e','l','i','t','.',' ','P','h','a','s','e','l','l','u','s',' ','v','e','n','e','n','a','t','i','s','.',
    // Backspace delete
    KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL, KEY_BKSPC_SOL,
    // Read only Input widget
    '\t',
    // ListBox Widget
    '\t',
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,
    KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP, KEY_UP,
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

extern "C" int __test_wgetch(void*) {
    static int* ptr2=__test_data;

    usleep(40000);

    if (*ptr2==0) {
        abort();
    }

    return *ptr2++;
}

// Event handler requires access
Input<>* ifixed;
Input<>* idyn;
ListBox<>* listbox;
Button* b1;
Button* b2;
Button* b3;

void button_press_handler(Event& _e) {
    assert(_e==EVT_BUTTON_PRESS);

    EventEx<Button*>& ev=dynamic_cast<EventEx<Button*>&>(_e);

    if (ev.data() == b1) {
        Curses::statusline()->push_msg(ifixed->input());
        return;
    }

    if (ev.data() == b2) {
        Curses::statusline()->push_msg(idyn->input());
        return;
    }

    if (ev.data() == b3) {
        EventQueue::submit(EVT_QUIT);
        return;
    }

    return;
}

int main() {
    std::list<std::string> items;

    for (int i=0; i<120; i++) {
        std::ostringstream n;
        n<<i;
        items.push_back("Long Name ListBox Item Number " + n.str());
    }

#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        Curses::init();

        EventQueue::connect_event(EventConnectorFunction1(EVT_BUTTON_PRESS,button_press_handler));

        Curses::title(new LineObject(LineObject::POS_TOP,
                                     "Focus 1"));
        Curses::statusline(new StatusLine);

        Curses::mainwindow(new Window(Margin(1,0,1,0)));
        Curses::mainwindow()->frame(true);

        VPack* vpack=new VPack;
        HPack* hpack=new HPack;
        b1=new Button("Button1");
        b2=new Button("Button2");
        b3=new Button("Button3");
        ifixed=new Input<>(10);
        idyn=new Input<>;
        listbox=new ListBox<>;
        listbox->set(items);
        Input<>* ireadonly=new Input<>;
        ireadonly->input("Read only");
        ireadonly->readonly(true);

        vpack->add_back(ifixed);
        vpack->add_back(idyn);
        vpack->add_back(ireadonly);
        vpack->add_back(listbox);
        vpack->hinting(false);

        hpack->add_back(b1);
        hpack->add_back(b2);
        hpack->add_back(b3);
        hpack->hinting(false);

        vpack->add_back(hpack);

        assert(b1->label()=="Button1");
        assert(b2->label()=="Button2");
        assert(b3->label()=="Button3");

        Curses::mainwindow()->widget(vpack);

        Curses::run();

        delete b1;
        delete b2;
        delete b3;
        delete idyn;
        delete ifixed;
        delete ireadonly;
        delete listbox;
        delete hpack;
        delete vpack;
        delete Curses::mainwindow();
        delete Curses::title();
        delete Curses::statusline();
        Curses::end();
    } catch (std::exception& e) {
        Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
