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

#ifndef COLORS_H
#define COLORS_H 1

#include <vector>
#include <string>

#include "mycurses.h"

namespace YACURS {
    namespace INTERNAL {
        /**
         * Struct holding all information associated with Color
         * Object.
         */
        struct CursColor {
            /**
             * Number of the pair.
             *
             * The number of the pair as used by (N)Curses. For
             * NCurses, it has to start with at 1. X/Open Curses
             * allows it to start at zero.
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
            CursColor() : no(-1),
                fg(-1),
                bg(-1),
                attr(-1) {
            }
        };
    } // namespace INTERNAL

    /**
     * Color Objects
     *
     * Color Object Names.
     *
     * @internal
     *
     *  Used as indices of the Color array.
     */
    enum COLOROBJ {
        /**
         * Default color.
         */
        DEFAULT = 0,

        DIALOG,

        DIALOG_TITLE,

        /**
         * Color of Input Widget without focus.
         */
        INPUTWIDGET_NOFOCUS,

        /**
         * Color of Input Widget with focus.
         */
        INPUTWIDGET_FOCUS,

        /**
         * Color of hidden text in Input Widget.
         */
        INPUTWIDGET_HIDDEN,

        /**
         * Color of Button without focus.
         */
        BUTTON_NOFOCUS,

        /**
         * Color of Button with focus.
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
         * Color of Title Bar
         */
        TITLEBAR,

        /**
         * Color of Status Bar
         */
        STATUSBAR,

        /**
         * Number of Colors.
         *
         * @internal
         *
         * Used by Colors to initialize color pairs as sentinel.
         */
        NUMBER_OF_COLOROBJ
    };

    /**
     * Initalizing colors and preparing colors
     */
    class Colors {
        private:
            /**
             * Flag, whether or not colors have been initialized.
             */
            static bool __initialized;

            /**
             * Array holding CursColor
             *
             * @internal
             *
             * The color pairs for curses are access by using the
             * names defined in the @c COLOR enum.
             */
            static std::vector<INTERNAL::CursColor> __colors;

        public:
            /**
             * Initializes Colors
             *
             * Initializes the color pairs used by curses if the
             * terminal supports colors.
             */
            static void init_colors(const std::string& colorstr=
                                        std::string() );

            static int color_pair(COLOROBJ c);
    };
} // namespace YACURS

#endif // COLORS_H
