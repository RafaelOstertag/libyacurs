// -*- mode: c++ -*-
//
// $Id$

#ifndef HPACK_H
#define HPACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pack.h"

namespace YACURS {
    /**
     * Packs Widgets horizontally.
     *
     * The main purpose of this class is the implement the size
     * calculation of and for the associated Widgets and to implement
     * realize().
     */
    class HPack: public Pack {
	private:
	    // Not supported
	    HPack& operator=(const HPack&);

	protected:
	    void recalc_size();
	    Size calc_size_non_dynamic() const;

	public:
	    virtual ~HPack();


	    Size size_hint() const;

	    void realize();
    };
}

#endif // HPACK_H
