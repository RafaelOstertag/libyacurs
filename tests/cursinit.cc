// $Id$
//
// Test initialization
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#include <iostream>

#include "yacurs.h"

int
main() {
    try {
        YACURS::Curses::init();
        YACURS::Curses::end();
    } catch (std::exception&e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
