// -*- mode: c++ -*-
//
// $Id: pack.h 4680 2013-01-14 21:36:10Z rafisol $

#ifndef HPACK_H
#define HPACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pack.h"

class HPack: public Pack {
    private:
	void resize(const Size&) { abort(); }
    protected:
	void add_size(const WidgetBase* _w);
	void recalc_size();

    public:
	HPack();
	HPack(const HPack& _hp);
	~HPack();
	const HPack& operator=(const HPack& _hp);

	WidgetBase* clone() const;
};

#endif // HPACK_H
