// $Id$

#include <cassert>
#include <stdexcept>

#include "size.h"

//
// Static
//
Size Size::__zero=Size(0,0);

//
// Private
//

//
// Protected
//

//
// Public
//

Size::Size(int _rows, int _cols) : __rows(_rows), __cols(_cols) {
    if (__rows < 0)
	throw std::out_of_range("Rows cannot be <0");
    if (__cols < 0)
	throw std::out_of_range("Columns cannot be <0");
}

int
Size::cols() const {
    return __cols;
}

int
Size::rows() const {
    return __rows;
}

void
Size::cols(int _cols) {
    if (_cols < 0)
	throw std::out_of_range("Columns cannot be <0");
     __cols = _cols;
}

void
Size::rows(int _rows) {
    if (_rows < 0)
	throw std::out_of_range("Rows cannot be <0");
    __rows = _rows;
}

const Size&
Size::operator+=(const Size& rhs) {
    assert(rhs.__cols>=0);
    assert(rhs.__rows>=0);
    __cols += rhs.__cols;
    __rows += rhs.__rows;
    return *this;
}

const Size&
Size::operator-=(const Size& rhs) {
    __cols -= rhs.__cols;
    if (__cols<0) __cols=0;

    __rows -= rhs.__rows;
    if (__rows<0) __rows=0;
    return *this;
}

Size
Size::operator+(const Size& rhs) const {
    Size tmp = *this;
    return tmp+=rhs;
}

Size
Size::operator-(const Size& rhs) const {
    Size tmp = *this;
    return tmp-=rhs;
}

bool
Size::operator==(const Size& rhs) const {
    return __cols == rhs.__cols && __rows == rhs.__rows;
}

bool
Size::operator!=(const Size& rhs) const {
    return !operator==(rhs);
}

const Size&
Size::zero() {
    return __zero;
}
