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
StatusLine::putTopMsg() {
    if (messages.empty())
	line(std::string());
    else
	line(messages.top());
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
StatusLine::pushMsg(const std::string& m) {
    messages.push(m);
    putTopMsg();
}

void
StatusLine::popMsg() {
    if (messages.empty()) return;
    messages.pop();
    putTopMsg();
}
