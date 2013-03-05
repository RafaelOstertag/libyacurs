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
int __test_data[]= { 
    // Remove buttons by removing them 
    '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n',
    // Select AGAIN
    '\t', '\n', 0
};

int __test_data2[] = {
    '\n', 0
};

extern "C" int __test_wgetch(void*) {
    static int round = 0;
    static int* ptr2=__test_data;

    if (*ptr2==0 && round < 500) {
	ptr2=__test_data;
	round++;
    }

    if (round >= 500 && *ptr2==0) {
	ptr2=__test_data2;
    }

    usleep(100);
    if (*ptr2==0) {
	abort();
    }
    return *ptr2++;
}

class MainWindow: public Window {
    private:
	VPack* vpack1;
	HPack* hpack1;
	HPack* hpack2;
	HPack* hpack3;
	Button* button1;
	Button* button2;
	Button* button3;
	Button* button4;
	Button* button5;
	Button* button6;
	Button* button7;
	Button* button8;
	Button* button9;
	Button* bagain;
	Button* bquit;
	
	void add_buttons() {
	    hpack1->add_back(button1);
	    hpack1->add_back(button2);
	    hpack1->add_back(button3);
	    hpack1->add_back(button4);
	    hpack1->add_back(button5);
	    hpack2->add_back(button6);
	    hpack2->add_back(button7);
	    hpack2->add_back(button8);
	    hpack2->add_back(button9);
	}

    protected:
	void button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

	    if (e.data()==button1) {
		hpack1->remove(button1);
	    }

	    if (e.data()==button2) {
		hpack1->remove(button2);
	    }

	    if (e.data()==button3) {
		hpack1->remove(button3);
	    }

	    if (e.data()==button4) {
		hpack1->remove(button4);
	    }

	    if (e.data()==button5) {
		hpack1->remove(button5);
	    }

	    if (e.data()==button6) {
		hpack2->remove(button6);
	    }

	    if (e.data()==button7) {
		hpack2->remove(button7);
	    }

	    if (e.data()==button8) {
		hpack2->remove(button8);
	    }

	    if (e.data()==button9) {
		hpack2->remove(button9);
	    }

	    if (e.data()==bagain) {
		add_buttons();
		hpack3->remove(bagain);
		return;
	    }


	    if (hpack2->widgets() == 0 &&
		hpack1->widgets() == 0 )
		hpack3->add_front(bagain);
		

	    if (e.data()==bquit) {
		EventQueue::submit(EVT_QUIT);
		return;
	    }
	}

    public:
	MainWindow(): Window(Margin(1,0,1,0)) {
	    button1=new Button("Button1");
	    button2=new Button("Button2");
	    button3=new Button("Button3");
	    button4=new Button("Button4");
	    button5=new Button("Button5");
	    button6=new Button("Button6");
	    button7=new Button("Button7");
	    button8=new Button("Button8");
	    button9=new Button("Button9");
	    
	    bagain=new Button("AGAIN");
	    bquit=new Button("Quit");
	    vpack1=new VPack();
	    hpack1=new HPack();
	    hpack2=new HPack();
	    hpack3=new HPack();

	    add_buttons();

	    hpack3->add_back(bquit);
	    vpack1->add_back(hpack1);
	    vpack1->add_back(hpack2);
	    vpack1->add_back(hpack3);

	    widget(vpack1);

	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	}

	~MainWindow() {
	    delete button1;
	    delete button2;
	    delete button3;
	    delete button4;
	    delete button5;
	    delete button6;
	    delete button7;
	    delete button8;
	    delete button9;
	    delete bquit;
	    delete hpack1;
	    delete hpack2;
	    delete hpack3;
	    delete vpack1;

	    EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
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
				     "Packremove"));
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
