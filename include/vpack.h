// -*- mode: c++ -*-
//
// $Id$

#ifndef VPACK_H
#define VPACK_H

#include "pack.h"

namespace YACURS {
    /**
     * Packs Widgets vertically.
     *
     * The main purpose of this class is the implement the size
     * calculation of and for the associated Widgets and to implement
     * realize().
     */
    class VPack: public Pack {
	private:
	    // Not supported
	    VPack& operator=(const VPack&);

	protected:
	    void recalc_size();
	    Size calc_size_non_dynamic() const;

	public:
	    virtual ~VPack();

	    Size size_hint() const;

	    void realize();
    };
}

#endif // VPACK_H
