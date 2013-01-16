// $Id: vpack.cc 4684 2013-01-15 20:47:55Z rafisol $

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>

#include "hpack.h"

// Functors
class CalcSize {
    private:
	Rectangle __size;
    public:
	CalcSize(Rectangle _s = Rectangle()): __size(_s) {}
	CalcSize(const CalcSize& _rs): __size(_rs.__size) {}
	void operator()(const Widget* _w) {
	    __size.rows(__size.rows() < _w->size().rows() ?
			_w->size().rows() : __size.rows());

	    __size.cols(__size.cols()+_w->size().cols());
	}
	const Rectangle& size() const { return __size; }
};

//
// Private
//

//
// Protected
//
void 
HPack::add_size(const Widget* _w) {
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
HPack::HPack(): Pack() {}

HPack::HPack(const HPack& _hp): Pack(_hp) {}

HPack::~HPack() {}

const HPack&
HPack::operator=(const HPack& _hp) {
    Pack::operator=(_hp);
    return *this;
}

Widget*
HPack::clone() const {
    return new HPack(*this);
}
