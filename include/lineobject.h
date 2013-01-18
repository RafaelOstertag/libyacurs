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
 * @todo provide options for centering and right aligning so that title can be
 * centered if it is used as title for the application.
 */
class LineObject: public WindowBase {
    public:
	enum POSITION {
	    POS_TOP,
	    POS_BOTTOM
	};

    private:
	std::string linetext;
	POSITION position;

	/**
	 * Compute and sets the margin of WindowBase to achieve the position
	 * of the line object according to @c pos.
	 *
	 * @param _s the screen size. If this parameter is equal to
	 * Area(), computeMargin calls Curses::inquiryScreenSize() in
	 * order to determine the screen size.
	 */
	void computeMargin(const Area& _s=Area());

    protected:
	void putLine();
	virtual int refresh_handler(Event& _e);
	virtual int resize_handler(Event& _e);

    public:
	LineObject(POSITION _pos, const std::string& _t = std::string());
	~LineObject();
	LineObject(const LineObject& lo);
	LineObject& operator=(const LineObject& lo);

	void realize();

	void line(const std::string& _str);
	std::string line() const;
};

#endif // LINEOBJECT_H
