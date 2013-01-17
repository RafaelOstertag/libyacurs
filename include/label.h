// -*- mode: c++ -*-
//
// $Id: widget.h 4684 2013-01-15 20:47:55Z rafisol $

#ifndef LABEL_H
#define LABEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "widget.h"
#include "screenobject.h"


class Label: public Widget {
    private:
	std::string __label;

    public:
	Label(const std::string& _l=std::string());
	Label(const Label& _l);
	~Label();
	const Label& operator=(const Label& _l);

	Widget* clone() const;

	void label(const std::string& _l);
	const std::string& label() const;
};

#endif // LABEL_H