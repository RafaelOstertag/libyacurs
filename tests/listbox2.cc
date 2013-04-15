// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_TERMIOS_H
# include <termios.h>
#else // HAVE_TERMIOS_H
# ifdef HAVE_SYS_TERMIOS_H
#  include <sys/termios.h>
# endif // HAVE_SYS_TERMIOS_H
#endif // HAVE_TERMIOS_H

#include <iostream>
#include <sstream>

#include "yacurs.h"

// Used when preloading libtestpreload.so
int __test_data[]= {
    // Select Add button
    '\t',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '1',
    // Ok dialog
    '\t', '\n',

    // Focus ListBox
    KEY_LEFT,

    // Change sort order
    'o', 'o', 'o',

    // Jump at the end
    KEY_CTRL_E,

    // Select delete button
    '\t', '\t',

    // Delete Items
    '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n',

    // Focus add button
    KEY_LEFT,

    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '1',
    // Ok dialog
    '\t', '\n',

    // Focus ListBox
    KEY_LEFT,

    // Jump at the end, then beginn
    KEY_CTRL_E, KEY_CTRL_A,

    // Select delete button
    '\t', '\t',

    // Delete Items
    '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n',

    // focus add button
    KEY_UP,

    //
    // Fill listbox
    //

    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '1',
    // Ok dialog
    '\t', '\n',

    // Select listbox
    KEY_LEFT,

    // Move down
    KEY_DOWN, KEY_DOWN, KEY_DOWN,

    // Some sorting
    'o', 'o', 'o', 'o', 'o', 'o',

    // Focus delete
    '\t', '\t',

    // Delete
    '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n',

    // Focus Add button
    KEY_LEFT,

    //
    // Fill listbox
    //

    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '1', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '1',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '2',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '3',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '4',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '5',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '6',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '7',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '8',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '2', '9',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '0',
    // Ok dialog
    '\t', '\n',
    // Press Add button
    '\n',
    'L','o','r','e','m',' ','i','p','s','u','m',' ', '3', '1',
    // Ok dialog
    '\t', '\n',

    //
    // Focus ListBox
    //
    KEY_LEFT,


    // Page down
    KEY_NPAGE, KEY_NPAGE, KEY_NPAGE,

    // Scroll somewhat more
    KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN, KEY_DOWN,

    // Scroll up
    KEY_UP,

    // Select Delete Button
    '\t', '\t',

    // Delete
    '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n',


    // Quit app
    '\t', '\t', '\t', '\n', 0
};

extern "C" int __test_wgetch(void*) {
    static int* ptr2=__test_data;

    usleep(10000);

    if (*ptr2==0) {
        abort();
    }

    return *ptr2++;
}

class MainWindow: public YACURS::Window {
    private:
        YACURS::Button* badd;
        YACURS::Button* bdelete;
	YACURS::Button* bsearch;
	YACURS::Button* bpreload;
        YACURS::Button* bquit;
        YACURS::VPack* vpack1;
        YACURS::HPack* hpack1;
        YACURS::ListBox<>* listbox;
        YACURS::InputBox* inputdialog;
	YACURS::InputBox* searchdialog;

    protected:
        void button_press_handler(YACURS::Event& _e) {
            assert(_e==YACURS::EVT_BUTTON_PRESS);

            YACURS::EventEx<YACURS::Button*>& _ex = dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

            if (bquit==_ex.data()) {
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                return;
            }

            if (badd==_ex.data()) {
		assert(inputdialog==0);
                inputdialog=new YACURS::InputBox("Add item", "String to add");
                inputdialog->show();
                return;
            }

	    if (bsearch==_ex.data()) {
		assert(searchdialog==0);
		searchdialog=new YACURS::InputBox("Search Item", "Enter search term");
		searchdialog->show();
		return;
	    }

            if (bdelete==_ex.data()) {
                listbox->delete_selected();
                return;
            }

            if (bpreload==_ex.data()) {
		std::list<std::string> tmp;
		for(int i=0; i<250; i++) {
		    std::ostringstream val;
		    val<<i;
		    tmp.push_back("Item " + val.str());
		}
                listbox->set(tmp);
                return;
            }
        }

        void window_close_handler(YACURS::Event& _e) {
            assert(_e==YACURS::EVT_WINDOW_CLOSE);

            YACURS::EventEx<YACURS::WindowBase*>& _ex = dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

            if (inputdialog==_ex.data()) {
                if (inputdialog->dialog_state()==YACURS::Dialog::DIALOG_OK) {
                    listbox->add(inputdialog->input());
                    YACURS::Curses::statusline()->pop_msg();
                    YACURS::Curses::statusline()->push_msg("Dialog OK");
                } else {
                    YACURS::Curses::statusline()->pop_msg();
                    YACURS::Curses::statusline()->push_msg("Dialog Cancelled");
                }

                delete inputdialog;
                inputdialog=0;
                return;
            }

	    if (searchdialog==_ex.data()) {
		if (searchdialog->dialog_state()==YACURS::Dialog::DIALOG_OK) {
		    listbox->high_light(searchdialog->input());
		}
		delete searchdialog;
		searchdialog=0;
		return;
	    }
        }

    public:
        MainWindow(const YACURS::Margin& _m): YACURS::Window(_m),
					      badd(0),
					      bdelete(0),
					      bsearch(0),
					      bpreload(0),
					      bquit(0),
					      vpack1(0),
					      hpack1(0),
					      listbox(0),
					      inputdialog(0),
					      searchdialog(0) {
            badd=new YACURS::Button("Add");
            bdelete=new YACURS::Button("Delete");
            bsearch=new YACURS::Button("Search");
	    bpreload=new YACURS::Button("Preload List");
            bquit=new YACURS::Button("Quit");

            vpack1=new YACURS::VPack;
            hpack1=new YACURS::HPack;

            listbox=new YACURS::ListBox<>;

            vpack1->add_back(badd);
            vpack1->add_back(bdelete);
	    vpack1->add_back(bsearch);
	    vpack1->add_back(bpreload);
            vpack1->add_back(bquit);

            hpack1->add_back(listbox);
            hpack1->add_back(vpack1);

            widget(hpack1);

            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
        }

        ~MainWindow() {
            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
            YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<MainWindow>(YACURS::EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
            delete badd;
            delete bdelete;
	    delete bsearch;
	    delete bpreload;
            delete bquit;
            delete vpack1;
            delete hpack1;
            delete listbox;
        }
};

int main() {
#if 0
    std::cout << getpid() << std::endl;
    sleep(15);
#endif

    try {
        YACURS::Curses::init();

        YACURS::LineObject* title = new YACURS::LineObject(YACURS::LineObject::POS_TOP,
                                           "ListBox 2");
        YACURS::Curses::title(title);

        MainWindow* w1 = new MainWindow(YACURS::Margin(1,0,1,0));
        w1->frame(true);

        YACURS::StatusLine* sl = new YACURS::StatusLine();
        YACURS::Curses::statusline(sl);

        YACURS::Curses::mainwindow(w1);

        YACURS::Curses::run();

        delete title;
        delete w1;
        delete sl;
        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
