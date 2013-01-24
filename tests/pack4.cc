// $Id$
//
// Test pack
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

// The alarm handler needs access to those
Label* label1;
Label* label3;

int alrm(Event& _e) {
    static int i=0;

    assert(_e == EVT_ALARM);
    switch (i++) {
    case 0:
	label1->label("Big New Label1");
	alarm(1);
	break;
	
    case 1:
	label1->label("Small label1");
	alarm(1);
	break;

    case 2:
	label1->label("This should resize");
	alarm(1);
	break;

    case 3:
	label3->label("Huge Big Label 3");
	alarm(1);
	break;
	
    default:
	EventQueue::submit(Event(EVT_QUIT));
	break;
    }
    return 0;
}

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "Pack 4");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);


	VPack* vpack = new VPack;
	HPack* hpack = new HPack;
	HPack* hpack1 = new HPack();
	HPack* hpack2 = new HPack();
	VPack* vpack1 = new VPack;
	VPack* vpack2 = new VPack;
	VPack* vpack2_1 = new VPack();
	VPack* vpack2_2 = new VPack();


	label1 = new Label("Test label");
	Label* label2 = new Label("Test label2");
	label3 = new Label("Test label3");
	Label* label4 = new Label("Test label4");
	Label* label5=new Label("Label 5");
	Label* label6=new Label("Label 6");
	Label* label7=new Label("Label 7");
	Label* label8=new Label("Label 8");
	Label* label9=new Label("Label 9");
	Label* label10=new Label("Label 10");
	Label* label11=new Label("Label 11");
	Label* label12=new Label("Label 12");
	Label* label13=new Label("Label 13");
	Label* label14=new Label("Label 14");
	Label* label15=new Label("Label 15");
	Label* label16=new Label("Label 16");

	vpack->add_front(hpack);
	vpack->add_front(hpack1);
	vpack->add_back(hpack2);

	hpack->add_back(vpack1);
	hpack->add_back(vpack2);

	hpack1->add_back(vpack2_1);
	hpack1->add_back(vpack2_2);

	vpack1->add_back(label1);
	vpack1->add_back(label2);
	vpack1->add_front(label3);

	vpack2->add_back(label4);
	vpack2->add_back(label5);
	vpack2->add_back(label6);

	vpack2_1->add_front(label7);
	vpack2_1->add_front(label8);
	vpack2_1->add_front(label9);

	vpack2_2->add_back(label10);
	vpack2_2->add_back(label11);
	vpack2_2->add_back(label12);
	vpack2_2->add_back(label13);
	vpack2_2->add_back(label14);

	hpack2->add_back(label15);
	hpack2->add_back(label16);

	label1->label("New Test Label");
	label2->label("New Test Label2");
	label3->label("New Test Label3");
	label4->label("New Test Label4");

	


	w1->widget(vpack);

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_ALARM,&alrm));

	alarm(5);
	Curses::run();

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

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
