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
    // Select Fill button,
    KEY_LEFT, '\n',
    // close window
    '\t', '\t', '\n',
    // Quit
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

class Win1: public Window {
    private:
	Label* l1;
	Input* i1;
	Button* close1;
	VPack* vp1;

    protected:
	void button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

	    if (e.data()==close1) {
		close();
	    }
	    return;
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

class ListBoxWin: public Window {
    private:
	VPack* vpack1;
	HPack* hpack1;
	ListBox<>* listbox;
	Button* bclear;
	Button* badd;
	Button* bclose;

    protected:
	void button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

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
	ListBoxWin(): Window(Margin(3,2,3,2)) {
	    frame(true);
	    listbox=new ListBox<>;
	    bclear=new Button("Clear");
	    badd=new Button("Fill");
	    bclose=new Button("Close");
	    vpack1=new VPack;
	    hpack1=new HPack;

	    hpack1->add_back(bclear);
	    hpack1->add_back(badd);
	    hpack1->add_back(bclose);
	    vpack1->add_back(listbox);
	    vpack1->add_back(hpack1);
	    
	    widget(vpack1);

	    EventQueue::connect_event(EventConnectorMethod1<ListBoxWin>(EVT_BUTTON_PRESS, this, &ListBoxWin::button_press_handler));
	}

	~ListBoxWin() {
	    delete listbox;
	    delete bclear;
	    delete bclose;
	    delete vpack1;
	    delete hpack1;

	    EventQueue::disconnect_event(EventConnectorMethod1<ListBoxWin>(EVT_BUTTON_PRESS, this, &ListBoxWin::button_press_handler));
	}
};

class MainWindow: public Window {
    private:
	HPack* hpack1;
	Button* button1;
	Button* button2;
	Button* button3;
	Win1* win1;
	ListBoxWin* lbwin;

	
    protected:
	void window_close_handler(Event& _e) {
	    assert(_e==EVT_WINDOW_CLOSE);
	    EventEx<WindowBase*>& evt=dynamic_cast<EventEx<WindowBase*>&>(_e);
	    if (win1!=NULL && evt.data()==win1) {
		Curses::statusline()->push_msg("Window 1 closed");
		delete win1;
		win1=NULL;
		return;
	    }

	    if (lbwin!=NULL && evt.data()==lbwin) {
		delete lbwin;
		lbwin=NULL;
		return;
	    }
	}

	void button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

	    if (e.data()==button1) {
		assert(win1==NULL);

		win1=new Win1;
		win1->show();
		return;
	    }

	    if (e.data()==button3) {
		assert(lbwin==NULL);

		lbwin=new ListBoxWin;
		lbwin->show();
		return;
	    }

	    if (e.data()==button2) {
		EventQueue::submit(EVT_QUIT);
		return;
	    }
	}

    public:
	MainWindow(): Window(Margin(1,0,1,0)), win1(NULL), lbwin(NULL) {
	    button1=new Button("New Window");
	    button2=new Button("Quit");
	    button3=new Button("List Box Win");
	    hpack1=new HPack();
	    hpack1->add_back(button1);
	    hpack1->add_back(button3);
	    hpack1->add_back(button2);
	    widget(hpack1);

	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
	}

	~MainWindow() {
	    if (win1)
		delete win1;

	    if (lbwin)
		delete lbwin;

	    delete button1;
	    delete button2;
	    delete button3;
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
