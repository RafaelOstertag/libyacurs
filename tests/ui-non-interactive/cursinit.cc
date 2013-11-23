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
#ifdef YACURS_USE_WCHAR
    if (setlocale(LC_ALL,"en_US.UTF-8")==0) exit(77);
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
