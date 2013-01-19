// -*- mode: c++ -*-

#ifndef CURS_H
#define CURS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRING
#include <string>
#endif // HAVE_STRING

#include "mycurses.h"
#include "area.h"
#include "lineobject.h"
#include "statusline.h"
#include "window.h"
#include "event.h"


class Curses {
    private:
	static StatusLine* __statusline;
	static LineObject* __title;
	static Window* __mainwindow;
	static bool initialized;

    protected:
	static int doupdate_handler(Event& e);
	static int termresetup_handler(Event& e);

    public:
	static void init();
	static void end();

	static void run();

	static void title(LineObject* _title);
	static LineObject* title();

	static void statusline(StatusLine* _sl);
	static StatusLine* statusline();

	static void mainwindow(Window* _w);
	static Window* mainwindow();

	static Size inquiryScreenSize();
};

#endif
