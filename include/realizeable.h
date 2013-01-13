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
	bool __realized;

    protected:
	virtual void realized(bool _r);

    public:
	Realizeable();
	Realizeable(const Realizeable& r);
	virtual ~Realizeable();

	Realizeable& operator=(const Realizeable& r);
	    
	virtual void refresh(bool immediate) = 0;
	/**
	 * Resizes the object. A resize must not refresh.
	 */
	virtual void resize(const Rectangle<>&) = 0;
	virtual void realize() = 0;
	bool realized() const;
};

#endif // REALIZEABLE_H
