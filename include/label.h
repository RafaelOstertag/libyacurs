// -*- mode: c++ -*-
//
// $Id$

#ifndef LABEL_H
#define LABEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>

#include <string>

#include "widget.h"

class Label: public Widget {
    private:
	std::string __label;
	Size __size;

    public:
	Label(const std::string& _l=std::string());
	Label(const Label& _l);
	~Label();
	const Label& operator=(const Label& _l);

	void label(const std::string& _l);
	const std::string& label() const;

	const Size& size() const;
	/**
	 * Does nothing.
	 *
	 * Does nothing on a Label, but we have to override.
	 */
	void resetsize();


	void refresh(bool immediate);
};

#endif // LABEL_H
