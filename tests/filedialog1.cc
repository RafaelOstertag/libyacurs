// $Id$
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "yacurs.h"

// Used when preloading libtestpreload.so
int __test_data_1[] = {
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

int __dir_up_data[] = {
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
int __test_data_end_dialog[]= { 
    '\t', '\t', '\t', '\n', 0
};

// Quit app
int __test_data_end_app[]= {
    '\t', '\n', 0
};

int* selection1[61];

int select_item() {
    int tmp;
    while ((tmp=(int)(drand48()*100)%20)<2);
    return tmp;
}

extern "C" int __test_wgetch(void*) {
    static int* ptr1=__test_data_1;
    static int* ptr_end_dialog = __test_data_end_dialog;
    static int* ptr_end_app = __test_data_end_app;
    static int selection_index = 0;
    static int selection_index2 = 0;

    usleep(70000);

    // Open dialog
    if (*ptr1!=0)
	return *ptr1++;

    if (selection1[selection_index]!=0) {
	if (selection1[selection_index][selection_index2]!=0) {
	    return selection1[selection_index][selection_index2++];
	} else {
	    if (selection1[selection_index+1]!=0)
		return selection1[++selection_index][selection_index2=0];
	}
    }

    if (*ptr_end_dialog!=0)
	return *ptr_end_dialog++;

    if (*ptr_end_app!=0)
	return *ptr_end_app++;

    abort();

    return 0;
}

class MainWindow: public Window {
    private:
	VPack* vpack1;
	HPack* hpack2;
	VPack* vpack21;
	VPack* vpack22;
	HPack* hpack1;
	Button* button1;
	Button* button2;
	Label* diagstatus;
	Label* file;
	Label* path;
	Label* filepath;
	DynLabel* label1;
	DynLabel* label2;
	DynLabel* label3;
	DynLabel* label4;
	FileDialog* filedialog;

	
    protected:
	void window_close_handler(Event& _e) {
	    assert(_e==EVT_WINDOW_CLOSE);
	    EventEx<WindowBase*>& evt=dynamic_cast<EventEx<WindowBase*>&>(_e);
	    if (filedialog!=0 && evt.data()==filedialog) {
		Curses::statusline()->push_msg("FileDialog closed");

		if (filedialog->dialog_state()==Dialog::DIALOG_OK) {
		    label1->label("DIALOG_OK");
		    label2->label(filedialog->filepath());
		    label3->label(filedialog->directory());
		    label4->label(filedialog->filename());
		} else {
		    label1->label("DIALOG_CANCEL");
		    label2->label("");
		    label3->label("");
		    label4->label("");
		}

		delete filedialog;
		filedialog=0;
	    }

	}

	void button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);
	    EventEx<Button*>& e=dynamic_cast<EventEx<Button*>&>(_e);

	    if (e.data()==button1) {
		assert(filedialog==0);

		filedialog=new FileDialog;
		filedialog->show();
		return;
	    }

	    if (e.data()==button2) {
		EventQueue::submit(EVT_QUIT);
		return;
	    }
	}

    public:
	MainWindow(): Window(Margin(1,0,1,0)), filedialog(0) {
	    button1=new Button("New Window");
	    button2=new Button("Quit");
	    vpack1=new VPack;
	    hpack1=new HPack;
	    label1=new DynLabel;
	    label2=new DynLabel;
	    label3=new DynLabel;
	    label4=new DynLabel;

	    diagstatus=new Label("Dialog Status:");
	    file=new Label("File:");
	    path=new Label("Path:");
	    filepath=new Label("File Path:");

	    hpack2=new HPack;
	    vpack21=new VPack;
	    vpack22=new VPack;

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
	    hpack1->add_back(button2);

	    vpack1->add_back(hpack2);
	    vpack1->add_back(hpack1);



	    widget(vpack1);

	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
	}

	~MainWindow() {
	    if (filedialog)
		delete filedialog;

	    delete button1;
	    delete button2;
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

	    EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	    EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
	}
};

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    srand48(time(0));

    // Initialize test data
    for(int i=0; i<30; i++) {
	if (i%2==0) {
	    int tmp = select_item();
	    selection1[i]=(int*)calloc(tmp+2, sizeof(int));
	    for(int n=0; n<tmp; n++)
		selection1[i][n]=KEY_DOWN;
	    
	    selection1[i][tmp]='\n';
	    selection1[i][tmp+1]=0;
	} else {
	    selection1[i]=(int*)calloc(21, sizeof(int));
	    memcpy(selection1[i],__dir_up_data,20*sizeof(int));
	    selection1[i][20]=0;
	}
    }
    selection1[60]=0;

    try {
	Curses::init();

	Curses::title(new LineObject(LineObject::POS_TOP,
				     "FileDialog 1"));
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

    for(int i=0; i<30; i++)
	free(selection1[i]);

    return 0;
}
