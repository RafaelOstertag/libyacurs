// -*- mode: c++ -*-
//
// $Id$

#ifndef STATUSLINE_H
#define STATUSLINE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stack>

#include "lineobject.h"

namespace YACURS {
    /**
     * Status Line.
     *
     * Maintains a stack of messages, where the top most message will
     * be displayed.
     */
    class StatusLine: public LineObject {
	private:
	    std::stack<std::string> __messages;
	
	    void put_top_msg();

	    StatusLine& operator=(const StatusLine&);
	public:
	    StatusLine();
	    virtual ~StatusLine();
	    void push_msg(const std::string& m);
	    void pop_msg();
    };
}

#endif
