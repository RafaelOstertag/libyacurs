// -*- mode: c++ -*-
//
// $Id$

#ifndef VPACK_H
#define VPACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pack.h"

/**
 * Packs Widgets vertically.
 *
 * The main purpose of this class is the implement the size
 * calculation of and for the associated Widgets and to implement
 * realize().
 */
class VPack: public Pack {
    protected:
	void recalc_size();
	Size calc_size_non_dynamic() const;

    public:
	VPack();
	VPack(const VPack& _vp);
	~VPack();
	const VPack& operator=(const VPack& _vp);

	Size size_hint() const;

	void realize();
};

#endif // VPACK_H
