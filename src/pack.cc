// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#include "pack.h"

// Functors
class CalcSize {
    private:
	Rectangle __size;

    public:
	CalcSize(): __size() {}
	CalcSize(const CalcSize& _cs): __size(cs.__size) {}
	const Rectangle& size() const { return __size; }
};

//
// Private
//

//
// Protected
//

//
// Public
//
Pack::Pack(): widget_list(), __size(), __area() {}

Pack::~Pack() {}

Pack::Pack(const Pack& _p):
    widget_list(_p.widget_list), __size(_p.__size), __area(_p.__area) {}

void
Pack::add_front(Widget* _w) {
    assert(_w != NULL);
    widget_list.push_front(_w);
    update_size()
}

void
Pack::add_back(Widget* _w) {
    assert(_w != NULL);
    widget_list.push_back(_w);
    update_size();
}

void
Pack::remove(Widget* _w) {
    assert(_w != NULL)
    widget_list.remove(_w);
}
    
const Rectangle&
Pack::size() {
    return __size;
}

void
Pack::area(const Rectangle& _a) {
    __area=_a;
}
    
const Rectangle&
Pack::area() const {
    return __area;
}
