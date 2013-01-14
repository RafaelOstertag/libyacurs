// -*- mode: c++ -*-
//
// $Id$

#ifndef WIDGET_H
#define WIDGET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "realizeable.h"
#include "rectangle.h"

class Widget: public Realizeable {
    public:
	virtual void size(const Rectangle& _size) = 0;
	virtual const Rectangle& size() const = 0;
	virtual Widget* clone() const = 0;

};

#endif // WIDGET_H
