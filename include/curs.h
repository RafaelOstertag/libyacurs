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
	static WINDOW* w;
	static StatusLine* statusline;
	static LineObject* title;
	static Window* mainwindow;
	static bool initialized;

    protected:
	static int doupdate_handler(Event& e);
	static int termresetup_handler(Event& e);

    public:
	static void init();
	static void end();

	static void run();

	static void setTitle(LineObject* _title);
	static LineObject* getTitle();
	static void unsetTitle();

	static void setStatusLine(StatusLine* _sl);
	static StatusLine* getStatusLine();
	static void unsetStatusLine();

	static void setWindow(Window* _w);
	static Window* getWindow();
	static void unsetWindow();

	static Size inquiryScreenSize();
};

#endif
