// -*- mode: c++ -*-
//
// $Id$

#ifndef HPACK_H
#define HPACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pack.h"

/**
 * Packs Widgets horizontally.
 *
 * The main purpose of this class is the implement the size
 * calculation of and for the associated Widgets and to implement
 * realize().
 */
class HPack: public Pack {
    protected:
	// Remember, this will be called from Pack::add_*()
	void recalc_size();

    public:
	HPack();
	HPack(const HPack& _hp);
	~HPack();
	const HPack& operator=(const HPack& _hp);

	void realize();
};

#endif // HPACK_H
