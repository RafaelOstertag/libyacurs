// -*- mode: c++ -*-
//
// $Id: pack.h 4680 2013-01-14 21:36:10Z rafisol $

#ifndef VPACK_H
#define VPACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pack.h"

class VPack: public Pack {
    private:
	Size __size;

    protected:
	void add_size(const WidgetBase* _w);
	void recalc_size();

    public:
	VPack();
	VPack(const VPack& _vp);
	~VPack();
	const VPack& operator=(const VPack& _vp);
	const Size& size() const;

	void realize();
};

#endif // VPACK_H
