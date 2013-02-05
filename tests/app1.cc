// $Id: focus1.cc 4810 2013-02-04 18:32:50Z rafisol $
//
// Test application
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <debug.h>
#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

class Win1: public Window {
    private:
	Label* l1;
	Input* i1;
	Button* close1;
	VPack* vp1;

    protected:
	int button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

	    if (e.data()==close1) {
		close();
	    }
	    return 0;
	}

    public:
	Win1(): Window(Margin(3,2,3,2)) {
	    frame(true);
	    l1 = new Label("Enter Text");
	    i1 = new Input;
	    close1 = new Button("Close");
	    vp1 = new VPack;
	    vp1->add_back(l1);
	    vp1->add_back(i1);
	    vp1->add_back(close1);
	    widget(vp1);

	    EventQueue::connect_event(EventConnectorMethod1<Win1>(EVT_BUTTON_PRESS, this, &Win1::button_press_handler));
	}

	~Win1() {
	    delete l1;
	    delete i1;
	    delete close1;
	    delete vp1;

	    EventQueue::disconnect_event(EventConnectorMethod1<Win1>(EVT_BUTTON_PRESS, this, &Win1::button_press_handler));
	}
};

class MainWindow: public Window {
    private:
	HPack* hpack1;
	Button* button1;
	Button* button2;
	Window* win1;

    protected:
	int window_close_handler(Event& _e) {
	    assert(_e==EVT_WINDOW_CLOSE);
	    EventWindowClose& evt=dynamic_cast<EventWindowClose&>(_e);

	    if (evt.data()!=win1) return 0;
	    Curses::statusline()->push_msg("Window 1 closed");
	    delete win1;
	    win1=NULL;
	    return 0;
	}

	int button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

	    if (e.data()==button1) {
		assert(win1==NULL);

		win1=new Win1;
		win1->show();
		return 0;
	    }

	    if (e.data()==button2) {
		EventQueue::submit(EVT_QUIT);
		return 0;
	    }

	    return 0;
	}

    public:
	MainWindow(): Window(Margin(1,0,1,0)), win1(NULL) {
	    button1=new Button("New Window");
	    button2=new Button("Quit");
	    hpack1=new HPack();
	    hpack1->add_back(button1);
	    hpack1->add_back(button2);
	    widget(hpack1);

	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
	}

	~MainWindow() {
	    if (win1)
		delete win1;

	    delete button1;
	    delete button2;
	    delete hpack1;

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
	DEBUG::start();
	Curses::init();


	Curses::title(new LineObject(LineObject::POS_TOP,
				     "App 1"));
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
