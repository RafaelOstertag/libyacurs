// -*- mode: c++ -*-
//
// $Id$

#ifndef LISTBOX_H
#define LISTBOX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <list>

#include "widget.h"

/**
 *
 */
class ListBox: public Widget {
    private:
	std::list<std::string> __list;

	/**
	 * Focus indication.
	 */
	bool __focus;

	/**
	 * Offset into the list.
	 */
	std::list<std::string>::size_type __offset;

	/**
	 * Position of the cursor in the visiable area
	 */
	std::list<std::string>::size_type __curs_pos;

	/**
	 * Size of the Widget.
	 *
	 * May be Size::zero() since dynamically size.
	 */
	Size __size;

    protected:
	virtual void key_handler(Event& _e);

	// From Realizeable
	void realize();
	
	void unrealize();

    public:
	/**
	 */
	ListBox();

	ListBox(const ListBox& _lb);

	virtual ~ListBox();

	ListBox& operator=(const ListBox& _i);

	/**
	 */
	virtual void add(const std::string& _i);

	virtual void set(const std::list<std::string>& _l);

	virtual void clear();

	virtual std::list<std::string>::size_type selected() const;

	// From WidgetBase
	
	void size_available(const Size& _s);

	/**
	 * Size the Input Widget requires.
	 *
	 * @return Either Size::zero() if __length is 0, or the
	 * Size(1, __length).
	 */
	Size size() const;

	Size size_hint() const;

	/**
	 * Dummy. Does nothing.
	 *
	 * Input Widget is not a container Widget, hence it may not
	 * notified of size changes().
	 *
	 * @return always @false
	 */
	bool size_change();

	/**
	 * Reset size.
	 *
	 * If __length is zero, resets __size. Else does nothing.
	 */
	void reset_size();

	bool can_focus() const;

	void focus(bool _f);

	bool focus() const;
	
	// From Realizeable
	/**
	 * Refresh the Input.
	 *
	 * @param immediate not directly used by Label::refresh() but
	 * passed to Widget::refresh().
	 */
	void refresh(bool immediate);
		
	operator std::string() const;
};

#endif // LISTBOX_H
