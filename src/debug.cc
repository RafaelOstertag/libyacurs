// $Id$

#include "debug.h"

bool DEBUG::__initialized=false;
std::ofstream DEBUG::__file;

void
DEBUG::start() {
    if (__initialized) return;

    __file.open("debug.out", std::ios::out | std::ios::trunc);
    __initialized=true;
}

void
DEBUG::end() {
    if (!__initialized) return;
    __file.close();
    __initialized=false;
}

void
DEBUG::out(const std::string& _o) {
    if (!__initialized) return;

    __file << _o << std::endl;
    __file.flush();
}
