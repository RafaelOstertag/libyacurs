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

#include "screenobject.h"


/**
 * A line object occupies one entire line of the screen.
 */
class LineObject: public ScreenObject {
    public:
	enum POSITION {
	    POS_TOP,
	    POS_BOTTOM
	};

    private:
	std::string linetext;
	POSITION position;

	/**
	 * Compute and sets the margin of ScreenObject to achieve the position
	 * of the line object according to \c pos.
	 *
	 * @param _s the screen size. If this parameter is equal to
	 * Rectangle<>(), computeMargin calls Curses::inquiryScreenSize() in
	 * order to determine the screen size.
	 */
	void computeMargin(const Rectangle<>& _s=Rectangle<>());

    protected:
	void putLine();
	virtual int refresh_handler(EventBase& _e);
	virtual int resize_handler(EventBase& _e);

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
