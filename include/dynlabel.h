// -*- mode: c++ -*-
//
// $Id: label.h 4786 2013-02-01 20:23:03Z rafisol $

#ifndef DYNLABEL_H
#define DYNLABEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "label.h"

/**
 * Display text on the screen.
 *
 * Simple widget for displaying text on the screen.
 *
 * As opposed to Label, DynLabel is dynamic, and dynamically adjust to
 * available space by truncating label text.
 */
class DynLabel: public Label {
    public:
	DynLabel(const std::string& _l=std::string());
	DynLabel(const DynLabel& _l);
	virtual ~DynLabel();
	DynLabel& operator=(const DynLabel& _l);

	// From Label
	void label(const std::string& _l);

	// From WidgetBase

	void size_available(const Size& _s);

	Size size_hint() const;

	void reset_size();

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

#endif // DYNLABEL_H
