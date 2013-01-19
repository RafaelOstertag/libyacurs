// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "cursex.h"
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

class SetPosAssoc {
    private:
	Coordinates __pos;
    public:
	SetPosAssoc(const Coordinates& __start): __pos(__start){}
	SetPosAssoc(const SetPosAssoc& _o): __pos(_o.__pos){}
	void operator()(WidgetBase* _w) {
	    _w->position(__pos);
	    // Set the size available
	    _w->size_available(_w->size());

	    __pos.y(__pos.y() + _w->size().rows());
	}
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
VPack::VPack(): Pack(), __size() {}

VPack::VPack(const VPack& _vp): Pack(_vp),
				__size(_vp.__size) {}

VPack::~VPack() {}

const VPack&
VPack::operator=(const VPack& _vp) {
    Pack::operator=(_vp);
    __size = _vp.__size;

    return *this;
}

const Size&
VPack::size() const {
    return __size;
}

void
VPack::realize() {
    if (realized()) throw AlreadyRealized();

    // Set position for each associated widget
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  SetPosAssoc(position()));

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::realize));

    realized(true);
}
