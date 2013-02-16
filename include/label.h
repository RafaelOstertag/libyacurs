// -*- mode: c++ -*-
//
// $Id$

#ifndef LABEL_H
#define LABEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "widget.h"

/**
 * Display text on the screen.
 *
 * Simple widget for displaying text on the screen.
 *
 * Label is not dynamic, i.e. if the text is too long, it might happen
 * that it cannot be realized() and throwing an exception.
 */
class Label: public Widget {
	// Not supported
	Label& operator=(const Label&);

    protected:
	/**
	 * The text to be displayed.
	 */
	std::string __label;

	/**
	 * The size the Label requires. Rows are always 1. Cols are
	 * __label.length(). Derrived classes may define other
	 * constraints.
	 */
	Size __size;

    public:
	/**
	 * Constructor.
	 *
	 * @param _l label
	 */
	Label(const std::string& _l=std::string());

	virtual ~Label();

	virtual void label(const std::string& _l);
	virtual std::string label() const;

	// From WidgetBase

	/**
	 * Size the Label requires.
	 *
	 * @return Size::rows() always 1. Size::cols() equal the
	 * length of the string to be displayed.
	 */
	Size size() const;

	Size size_hint() const;

	/**
	 * Dummy. Does nothing.
	 *
	 * Label is not a container Widget, hence it may not notified
	 * of size changes().
	 *
	 * @return always @false
	 */
	bool size_change();

	/**
	 * Does nothing.
	 *
	 * Does nothing on a Label, but we have to override.
	 */
	void reset_size();

	bool can_focus() const;
	void focus(bool);
	bool focus() const;

	// From Realizeable
	/**
	 * Refresh the label.
	 *
	 * Adds the Label text to the subwin.
	 *
	 * @param immediate not directly used by Label::refresh() but
	 * passed to Widget::refresh().
	 */
	void refresh(bool immediate);
};

#endif // LABEL_H
