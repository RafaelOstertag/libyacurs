// -*- mode: c++ -*-
//
// $Id$

#ifndef INPUT_H
#define INPUT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "widget.h"

/**
 * Input line.
 *
 * Widget for text input.
 */
class Input: public Widget {
    private:
	/**
	 * Focus indication.
	 */
	bool __focus;

	/**
	 * Offset into the buffer. Used when not the entire buffer can
	 * be displayed in the Input Widget, i.e. scrolling.
	 */
	std::string::size_type __offset;

	/**
	 * Cursor position relative to offset, absolute to Widget.
	 */
	std::string::size_type __curs_pos;

	/**
	 * Maximum size of input.
	 *
	 * If it is 0, there is no limit.
	 */
	std::string::size_type __max_size;

	/**
	 * Length of the input line.
	 *
	 * May be zero when dynamically sized.
	 */
	int __length;

	/**
	 * The input
	 */
	std::string __buffer;

	/**
	 * The size of the Input Widget.
	 *
	 * May be Size::zero() when dynamically sized, else the
	 */
	Size __size;

	/**
	 * Visibly change focus.
	 */
	void visibly_change_focus();

    protected:
	virtual int key_handler(Event& _e);

	// From Realizeable
	void realize();
	void unrealize();

    public:
	/**
	 * @param _length the length (columns) the widget requires. If
	 * zero, it will dynamically size. Default 0.
	 *
	 * @param _max_size maximum size of input. Default 255.
	 *
	 * @param _t text to display initially. Default empty.
	 */
	Input(int _length=0, std::string::size_type _max_size=255, const std::string& _t=std::string());
	Input(const Input& _i);
	~Input();
	Input& operator=(const Input& _i);

	/**
	 * Set the content of the input buffer.
	 *
	 * @param _i the content of the input buffer.
	 */
	virtual void input(const std::string& _i);

	/**
	 * Return the content of the input buffer.
	 *
	 * @return the content of the input buffer.
	 */
	virtual std::string input() const;

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
	void focus(bool);
	bool focus() const;

	// From Realizeable
	/**
	 * Refresh the Input.
	 *
	 * @param immediate not directly used by Label::refresh() but
	 * passed to Widget::refresh().
	 */
	void refresh(bool immediate);
};

#endif // INPUT_H
