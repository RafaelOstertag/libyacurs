// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>

#include "vpack.h"

// Functors
class CalcSize {
    private:
	Size __size;
    public:
	CalcSize(const Size& _s = Size()): __size(_s) {}
	CalcSize(const CalcSize& _rs): __size(_rs.__size) {}
	void operator()(const WidgetBase* _w) {
	    __size.rows(__size.rows() +
			_w->size().rows());
	    __size.cols(__size.cols() < _w->size().cols() ?
			_w->size().cols() : __size.cols());
	}
	const Size& size() const { return __size; }
};

//
// Private
//

//
// Protected
//
void 
VPack::add_size(const WidgetBase* _w) {
    assert(_w != NULL);
    CalcSize newsize(__size);
    newsize(_w);
    __size=newsize.size();
}

void
VPack::recalc_size() {
    CalcSize newsize;
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  newsize);

    __size = newsize.size();
}

//
// Public
//
VPack::VPack(): Pack() {}

VPack::VPack(const VPack& _vp): Pack(_vp) {}

VPack::~VPack() {}

const VPack&
VPack::operator=(const VPack& _vp) {
    Pack::operator=(_vp);
    return *this;
}

WidgetBase*
VPack::clone() const {
    return new VPack(*this);
}
