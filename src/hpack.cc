// $Id: vpack.cc 4684 2013-01-15 20:47:55Z rafisol $

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "cursex.h"
#include "hpack.h"

// Functors
class CalcSize {
    private:
	Size __size;
    public:
	CalcSize(const Size _s = Size()): __size(_s) {}
	CalcSize(const CalcSize& _rs): __size(_rs.__size) {}
	void operator()(const WidgetBase* _w) {
	    __size.rows(__size.rows() < _w->size().rows() ?
			_w->size().rows() : __size.rows());

	    __size.cols(__size.cols()+_w->size().cols());
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

	    __pos.x(__pos.x() + _w->size().cols() + 1);
	}
};


//
// Private
//

//
// Protected
//
void 
HPack::add_size(const WidgetBase* _w) {
    assert(_w != NULL);

    CalcSize newsize(__size);
    newsize(_w);

    __size=newsize.size();
}

void
HPack::recalc_size() {
    CalcSize newsize;

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  newsize);

    __size = newsize.size();
}

//
// Public
//
HPack::HPack(): Pack(), __size() {
}

HPack::HPack(const HPack& _hp): Pack(_hp),
				__size(_hp.__size) {
}

HPack::~HPack() {}

const HPack&
HPack::operator=(const HPack& _hp) {
    Pack::operator=(_hp);
    __size=_hp.__size;
    return *this;
}

const Size& 
HPack::size() const {
    return __size;
}

void
HPack::realize() {
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
