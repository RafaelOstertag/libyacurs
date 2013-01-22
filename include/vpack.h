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
 * The main purpose of this class is the implement the size calculation of and
 * for the associated Widgets and to implement realize().
 */
class VPack: public Pack {
    protected:
	// Remember, this will be called from Pack::add_*()
	void recalc_size();

    public:
	VPack();
	VPack(const VPack& _vp);
	~VPack();
	const VPack& operator=(const VPack& _vp);

	void realize();
};

#endif // VPACK_H
