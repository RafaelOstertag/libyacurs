// -*- mode: c++ -*-
//
// This file is part of libyacurs, 
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifndef CURS_H
#define CURS_H

#include <string>

#include "area.h"
#include "lineobject.h"
#include "statusline.h"
#include "window.h"
#include "event.h"
#include "mycurses.h"

namespace YACURS {

    /**
     * Resizing stops when any of those are underrun.
     */
    enum {
	/// Minimum columns. When actual screen size underruns MIN_COLS,
	/// no resize is performed.
	MIN_COLS=15,
	/// Minimum rows. When actual screen size underruns MIN_ROWS,
	/// no resize is performed.
	MIN_ROWS=15
    };

    /**
     * Static class for starting/stopping Curses.
     *
     * Class taking care of initializing and unitializing curses, and
     * running the application.
     *
     * It can take of showing a title, status line, and/or main window.
     */
    class Curses {
	private:
	    /**
	     * Pointer to StatusLine.
	     *
	     * Curses will not free the memory associated with StatuLine.
	     */
	    static StatusLine* __statusline;

	    /**
	     * Pointer to a title LineObject.
	     *
	     * Curses will not free the memory associated with LineObject.
	     */
	    static LineObject* __title;
	
	    /**
	     * Pointer to the main window.
	     *
	     * Curses will not free the memory associated with Window.
	     */
	    static Window* __mainwindow;

	    /**
	     * Flag indicating whether or not Curses has been initialized.
	     *
	     * It will be set by init() and removed by end().
	     */
	    static bool initialized;

	protected:
	    /**
	     * Handler for EVT_DOUPDATE event.
	     *
	     * @param e doupdate event.
	     */
	    static void doupdate_handler(Event& e);

	    static void termresetup_handler(Event& e);

	public:
	    static void init();
	    static void end();

	    /**
	     * Start the application.
	     *
	     * Starts the application by initializing the Focus Manager,
	     * showing the title, status line and main window (if any),
	     * and starting the EventQueue..
	     *
	     * Upon termination of the EventQueue by an EVT_QUIT event,
	     * the title, status line and main window (if any) will be
	     * closed and the Focus Manager will be uninitialized.
	     */
	    static void run();

	    static void title(LineObject* _title);
	    static LineObject* title();

	    static void statusline(StatusLine* _sl);
	    static StatusLine* statusline();

	    static void mainwindow(Window* _w);
	    static Window* mainwindow();

	    static Size inquiry_screensize();
    };
}

#endif
