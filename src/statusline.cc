// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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


//
// Protected
//

//
// Public
//

StatusLine::StatusLine():
    LineObject(POS_BOTTOM) {}

StatusLine::StatusLine(const StatusLine& sl):
    LineObject(sl) {}

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
