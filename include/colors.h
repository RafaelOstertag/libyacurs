// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _COLORS_H
#define _COLORS_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <vector>
#include <string>

#include "mycurses.h"

namespace YACURS {
    /**
     * @brief The indices of the color array.
     *
     * This are the indices of the color array.
     */
    enum COLOROBJ {
	/**
	 * The default color.
	 */
	DEFAULT = 0,

	/**
	 * The color used for the title of a message box.
	 */
	MESSAGEBOX_TITLE,

	/**
	 * The color of the message box
	 */
	MESSAGEBOX,

	/**
	 * The color for an input widget without focus.
	 */
	INPUTWIDGET_NOFOCUS,

	/**
	 * Color for an input widget with focus.
	 */
	INPUTWIDGET_FOCUS,

	/**
	 * Color for hidden text in input widget.
	 */
	INPUTWIDGET_HIDDEN,

	/**
	 * Color of buttons without focus.
	 */
	BUTTON_NOFOCUS,

	/**
	 * Color of buttons with focus.
	 */
	BUTTON_FOCUS,

	/**
	 * Color of ListBox
	 */
	LISTBOX,

	/**
	 * Color of ListBox with Focus
	 */
	LISTBOX_HILITE,

	/**
	 * Color of Check Box Groups
	 */
	CHECKBOXGROUP,

	/**
	 * Color of the Check Box Group Title
	 */
	CHECKBOXGROUP_TITLE,

	/**
	 * Number of Colors
	 */
	NUMBER_OF_COLOROBJ
    };

    /**
     * Struct holding the color.
     *
     * The purpose of this struct is to hold the color and the
     * attribute for the given color. The attribute is used in case
     * the terminal does not support colors.
     */
    struct CursColor {
            /**
             * Number of the pair.
             *
             * The number of the pair as used by (n)curses.
             */
            short no;
            /**
             * Foreground color.
             *
             * The foreground color of the color pair
             */
            short fg;
            /**
             * Background color.
             *
             * The background color of the pair.
             */
            short bg;
            /**
             * Attribute.
             *
             * The attribute used when no color is available.
             */
            int attr;

	    /**
	     * Initialize.
	     *
	     * Initialize all attributes to -1.
	     */
	    CursColor(): no(-1),
			 fg(-1),
			 bg(-1),
			 attr(-1) {}
    };

    /**
     * @brief Class for managing colors.
     *
     * This class is used for managing colors.
     */
    class Colors {
	private:
	    /**
	     * @brief Indicates whether or colors has been initialized
	     *
	     * Indicates whether or colors has been initialized by
	     * calling \c initColors().
	     */
	    static bool __initialized;
	    
	    /**
	     * @brief Array holding the colors.
	     *
	     * The color pairs for curses are access by using the
	     * names defined in the \c COLOR enum.
	     */
	    static std::vector<CursColor> __colors;


	public:
	    /**
	     * @brief Initializes the color pairs.
	     *
	     * Initializes the color pairs used by curses if the
	     * terminal supports colors.
	     */
	    static void init_colors(const std::string& colorstr=std::string());

	    static int color_pair(COLOROBJ c);
    };
}

#endif // _COLORS_H
