// $Id: widget7.cc 4732 2013-01-23 21:21:35Z rafisol $
//
// Test Pack
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yacurs.h"

// The alarm handler needs access to those
VPack* vpack;
HPack* hpack;
HPack* hpack1;
HPack* hpack2;
VPack* vpack1;
VPack* vpack2;
VPack* vpack2_1;
VPack* vpack2_2;


Label* label1;
Label* label2;
Label* label3;
Label* label4;
Label* label5;
Label* label6;
Label* label7;
Label* label8;
Label* label9;
Label* label10;
Label* label11;
Label* label12;
Label* label13;
Label* label14;
Label* label15;
Label* label16;

int alrm(Event& _e) {
    static int i=0;

    assert(_e == EVT_SIGALRM);
    switch (i++) {
    case 0:
	Curses::title()->line("Pack 9: always_dynamic=false, hinting=false");
	hpack->always_dynamic(false);
	hpack->hinting(false);
	hpack1->always_dynamic(false);
	hpack1->hinting(false);
	hpack2->always_dynamic(false);
	hpack2->hinting(false);
	vpack1->always_dynamic(false);
	vpack1->hinting(false);
	vpack2->always_dynamic(false);
	vpack2->hinting(false);
	vpack2_1->always_dynamic(false);
	vpack2_1->hinting(false);
	vpack2_2->always_dynamic(false);
	vpack2_2->hinting(false);
	alarm(1);
	break;

    case 1:
	Curses::title()->line("Pack 9: always_dynamic=true, hinting=false");
	hpack->always_dynamic(true);
	hpack->hinting(false);
	hpack1->always_dynamic(true);
	hpack1->hinting(false);
	hpack2->always_dynamic(true);
	hpack2->hinting(false);
	vpack1->always_dynamic(true);
	vpack1->hinting(false);
	vpack2->always_dynamic(true);
	vpack2->hinting(false);
	vpack2_1->always_dynamic(true);
	vpack2_1->hinting(false);
	vpack2_2->always_dynamic(true);
	vpack2_2->hinting(false);
	alarm(1);
	break;

    case 2:
	Curses::title()->line("Pack 9: always_dynamic=false, hinting=true");
	hpack->always_dynamic(false);
	hpack->hinting(true);
	hpack1->always_dynamic(false);
	hpack1->hinting(true);
	hpack2->always_dynamic(false);
	hpack2->hinting(true);
	vpack1->always_dynamic(false);
	vpack1->hinting(true);
	vpack2->always_dynamic(false);
	vpack2->hinting(true);
	vpack2_1->always_dynamic(false);
	vpack2_1->hinting(true);
	vpack2_2->always_dynamic(false);
	vpack2_2->hinting(true);
	alarm(1);
	break;

    case 3:
	Curses::title()->line("Pack 9: always_dynamic=true, hinting=true");
	hpack->always_dynamic(true);
	hpack->hinting(true);
	hpack1->always_dynamic(true);
	hpack1->hinting(true);
	hpack2->always_dynamic(true);
	hpack2->hinting(true);
	vpack1->always_dynamic(true);
	vpack1->hinting(true);
	vpack2->always_dynamic(true);
	vpack2->hinting(true);
	vpack2_1->always_dynamic(true);
	vpack2_1->hinting(true);
	vpack2_2->always_dynamic(true);
	vpack2_2->hinting(true);
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
					   "Pack 9:");
	Curses::title(title);

	Window* w1 = new Window(Margin(1,0,0,0));
	w1->frame(true);


	vpack = new VPack;
	hpack = new HPack;
	hpack1 = new HPack();
	hpack2 = new HPack();
	vpack1 = new VPack;
	vpack2 = new VPack;
	vpack2_1 = new VPack();
	vpack2_2 = new VPack();


	label1 = new Label("Test label");
	label2 = new Label("Test label2");
	label3 = new Label("Test label3");
	label4 = new Label("Test label4");
	label5 = new Label("Label 5");
	label6 = new Label("Label 6");
	label7 = new Label("Label 7");
	label8 = new Label("Label 8");
	label9 = new Label("Label 9");
	label10 = new Label("Label 10");
	label11 = new Label("Label 11");
	label12 = new Label("Label 12");
	label13 = new Label("Label 13");
	label14 = new Label("Label 14");
	label15 = new Label("Label 15");
	label16 = new Label("Label 16");

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

	Curses::mainwindow(w1);

	EventQueue::connect_event(EventConnectorFunction1(EVT_SIGALRM,&alrm));

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
