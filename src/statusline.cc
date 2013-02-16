// $Id$

#include <cstdlib>

#include "curs.h"
#include "cursex.h"
#include "statusline.h"
//
// Private
//

void
StatusLine::put_top_msg() {
    if (__messages.empty())
	line(std::string());
    else
	line(__messages.top());
}

StatusLine&
StatusLine::operator=(const StatusLine&) {
    abort();
    return *this;
}
//
// Protected
//

//
// Public
//

StatusLine::StatusLine():
    LineObject(POS_BOTTOM) {}



StatusLine::~StatusLine() {}

void
StatusLine::push_msg(const std::string& m) {
    __messages.push(m);
    put_top_msg();
}

void
StatusLine::pop_msg() {
    if (__messages.empty()) return;
    __messages.pop();
    put_top_msg();
}
