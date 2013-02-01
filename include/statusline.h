// -*- mode: c++ -*-
//
// $Id$

#ifndef STATUSLINE_H
#define STATUSLINE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STACK
#include <stack>
#endif // HAVE_STACK

#include "lineobject.h"

class StatusLine: public LineObject {
    private:
	std::stack<std::string> __messages;
	
	void put_top_msg();

    public:
	StatusLine();
	StatusLine(const StatusLine& sl);
	virtual ~StatusLine();
	void push_msg(const std::string& m);
	void pop_msg();
};

#endif
