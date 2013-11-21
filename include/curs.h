// -*- mode: c++ -*-
//
// This file is part of libyacurs.
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
#define CURS_H 1

#include <string>

#include "area.h"
#include "titlebar.h"
#include "statusbar.h"
#include "window.h"
#include "event.h"
#include "yacurscurses.h"

/**
 * General name space of libyacurs.
 */
namespace YACURS {
    /**
     * Static class for starting/stopping Curses.
     *
     * Class taking care of initializing and unitializing curses, and
     * running the application.
     *
     * It takes care of showing a title, status line, and/or main
     * window.
     */
    class Curses {
        private:
            /**
             * Screen size before suspending.
             *
             * Before suspending on SIGTSTP, the current screen size
             * will be saved. When resumed by SIGCONT, the current
             * screen size will be compared to this, and a resize
             * performed if they differ.
             */
            static Size suspend_scrsz;

            /**
             * Pointer to StatusBar.
             *
             * Curses will not free the memory associated with StatuLine.
             */
            static StatusBar* __statusbar;

            /**
             * Pointer to a title LineObject.
             *
             * Curses will not free the memory associated with LineObject.
             */
            static TitleBar* __title;

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

            /**
             * Flag indicating whether or not program has been suspended.
             */
            static volatile bool __suspended;

	    /**
	     * The terminals we know
	     *
	     * Holds the terminals we know that can have set the
	     * title.
	     */
	    static const char* __xterm_list[];

	    static bool is_xterm();

        protected:
            /**
             * Handler for EVT_DOUPDATE event.
             *
             * @param e doupdate event.
             */
            static void doupdate_handler(Event& e);

            static void termresetup_handler(Event& e);

            static void sigtstp_handler(Event& e);

            static void sigcont_handler(Event& e);

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

            static void title(TitleBar* _title);

            static TitleBar* title();

            static void statusbar(StatusBar* _sl);

            static StatusBar* statusbar();

            static void mainwindow(Window* _w);

            static Window* mainwindow();

            static Size current_screensize();

            static Size inquiry_screensize();

	    static void set_terminal_title(const std::string& _str);
    };
}

#endif
