// -*- mode: c++ -*-
//
// $Id$

#ifndef REALIZEABLE_H
#define REALIZEABLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rectangle.h"

class Realizeable {
    private:
	bool realized;

    protected:
	virtual void setRealized(bool _r);

    public:
	Realizeable();
	Realizeable(const Realizeable& r);
	virtual ~Realizeable();

	Realizeable& operator=(const Realizeable& r);
	    
	virtual void refresh() = 0;
	virtual void resize(const Rectangle<>&) = 0;
	virtual void realize(const Rectangle<>&) = 0;
	bool isRealized() const;
};

#endif // REALIZEABLE_H
