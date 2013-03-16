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
    '\t', '\n', 0
};

extern "C" int __test_wgetch(void*) {
    static int* ptr2=__test_data;

    usleep(10000);
    if (*ptr2==0) {
	abort();
    }
    return *ptr2++;
}

class MainWindow: public Window {
    private:
	Button* badd;
	Button* bdelete;
	Button* bquit;
	VPack* vpack1;
	HPack* hpack1;
	ListBox<>* listbox;
	InputBox* inputdialog;

    protected:
	void button_press_handler(Event& _e) {
	    assert(_e==EVT_BUTTON_PRESS);

	    EventEx<Button*>& _ex = dynamic_cast<EventEx<Button*>&>(_e);

	    if (bquit==_ex.data()) {
		EventQueue::submit(EVT_QUIT);
		return;
	    }

	    if (badd==_ex.data()) {
		inputdialog=new InputBox("Add item", "String to add");
		inputdialog->show();
		return;
	    }

	    if (bdelete==_ex.data()) {
		listbox->delete_selected();
		return;
	    }
	}

	void window_close_handler(Event& _e) {
	    assert(_e==EVT_WINDOW_CLOSE);

	    EventEx<WindowBase*>& _ex = dynamic_cast<EventEx<WindowBase*>&>(_e);

	    if (inputdialog==_ex.data()) {
		if (inputdialog->dialog_state()==Dialog::DIALOG_OK) {
		    listbox->add(inputdialog->input());
		    Curses::statusline()->pop_msg();
		    Curses::statusline()->push_msg("Dialog OK");
		} else {
		    Curses::statusline()->pop_msg();
		    Curses::statusline()->push_msg("Dialog Cancelled");
		}

		delete inputdialog;
		inputdialog=0;
		return;
	    }
	}

    public:
	MainWindow(const Margin& _m): Window(_m),
				      badd(0),
				      bdelete(0),
				      bquit(0),
				      vpack1(0),
				      hpack1(0),
				      listbox(0),
				      inputdialog(0) {
	    badd=new Button("Add");
	    bdelete=new Button("Delete");
	    bquit=new Button("Quit");

	    vpack1=new VPack;
	    hpack1=new HPack;

	    listbox=new ListBox<>;

	    vpack1->add_back(badd);
	    vpack1->add_back(bdelete);
	    vpack1->add_back(bquit);

	    hpack1->add_back(listbox);
	    hpack1->add_back(vpack1);

	    widget(hpack1);

	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	    EventQueue::connect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
	}

	~MainWindow() {
	    EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_BUTTON_PRESS, this, &MainWindow::button_press_handler));
	    EventQueue::disconnect_event(EventConnectorMethod1<MainWindow>(EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));
	    delete badd;
	    delete bdelete;
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
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   "ListBox 2");
	Curses::title(title);

	MainWindow* w1 = new MainWindow(Margin(1,0,1,0));
	w1->frame(true);

	StatusLine* sl = new StatusLine();
	Curses::statusline(sl);

	Curses::mainwindow(w1);

	Curses::run();

	delete title;
	delete w1;
	delete sl;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
