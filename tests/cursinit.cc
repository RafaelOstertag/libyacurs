// $Id$
//
// Test initialization
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <iostream>

#include "yacurs.h"

int
main() {
#ifdef USE_WCHAR
    setlocale(LC_ALL, "");
#endif

    try {
        YACURS::Curses::init();
        YACURS::Curses::end();
    } catch (std::exception&e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
