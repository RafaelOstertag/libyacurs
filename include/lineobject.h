// -*- mode: c++ -*-
//
// $Id$

#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRING
#include <string>
#endif // HAVE_STRING

#include "windowbase.h"


/**
 * A line object occupies one entire line of the screen.
 *
 * @todo provide options for centering and right aligning so that
 * title can be centered if it is used as title for the application.
 */
class LineObject: public WindowBase {
    public:
	enum POSITION {
	    POS_TOP,
	    POS_BOTTOM
	};

    private:
	std::string __linetext;
	POSITION __position;

	/**
	 * Compute and sets the margin of WindowBase to achieve the
	 * position of the line object according to @c pos.
	 */
	void compute_margin();

    protected:
	void put_line();

    public:
	LineObject(POSITION _pos, const std::string& _t = std::string());
	virtual ~LineObject();
	LineObject(const LineObject& lo);
	LineObject& operator=(const LineObject& lo);

	void realize();
	void refresh(bool immediate);

	void line(const std::string& _str);
	std::string line() const;

	operator std::string();
};

#endif // LINEOBJECT_H
