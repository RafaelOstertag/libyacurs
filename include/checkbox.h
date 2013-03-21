// -*- mode: c++ -*-
//
// $Id: input.h 4875 2013-02-23 13:25:34Z rafisol $

#ifndef CHECKBOX_H
#define CHECKBOX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <vector>

#include "widget.h"

struct Selectable {
	bool selected;
	std::string item;

	/**
	 * Used when initializing vector.
	 */
	Selectable():
	    selected(false) {}

	Selectable(const std::string& _str):
	    selected(false),
	    item(_str) {}

	Selectable(const Selectable& _s):
	    selected(_s.selected),
	    item(_s.item) {}

	Selectable& operator=(const Selectable& _s) {
	    selected=_s.selected;
	    item=_s.item;
	    return *this;
	}
};


/**
 */
class CheckBox: public Widget {
    private:
	/**
	 * The size of the CheckBox
	 */
	Size __size;

	/**
	 *
	 */
	unsigned short __cursor;

	std::string __title;

	CheckBox& operator=(const CheckBox& _i);
    protected:
	std::string __indicators[2];

	std::vector<Selectable> __items;

	virtual void key_handler(Event& _e);
	virtual void set_selection(unsigned short _cursor);

    public:
	CheckBox(const std::string& _title,
		 const std::vector<std::string>& _items);

	virtual ~CheckBox();

	bool selected(unsigned short _i);
	bool selected(const std::string& _i);

	// From WidgetBase
	void size_available(const Size& _s);

	/**
	 */
	Size size() const;

	Size size_hint() const;

	/**
	 * Dummy. Does nothing.
	 *
	 * CheckBox is not a container Widget, hence it may not
	 * notified of size changes().
	 *
	 * @return always @false
	 */
	bool size_change();

	/**
	 * Reset size.
	 *
	 * Does nothing.
	 */
	void reset_size();

	// From Realizeable
	/**
	 * Refresh the Input.
	 *
	 * @param immediate not directly used by Label::refresh() but
	 * passed to Widget::refresh().
	 */
	void refresh(bool immediate);

	void realize();
	void unrealize();
};

#endif // CHECKBOX_H
