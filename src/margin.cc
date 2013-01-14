// $Id$

#include "margin.h"

//
// Private
//

//
// Protected
//
int
Margin::max(int a, int b) const {
    return a>b ? a : b;
}

int
Margin::min(int a, int b) const {
    return a<b ? a : b;
}

//
// Public
//
Margin::Margin(int _top, int _left, int _bottom, int _right) :
    __top(_top),
    __bottom(_bottom),
    __left(_left),
    __right(_right) {}

Margin::Margin():
    __top(0), __bottom(0), __left(0), __right(0) {}

Margin::Margin(const Margin& m) {
    __top = m.__top;
    __bottom = m.__bottom;
    __left = m.__left;
    __right = m.__right;
}

Margin::~Margin() {}

void
Margin::top(int i) {
    __top = i;
}

void
Margin::bottom(int i) {
    __bottom = i;
}

void
Margin::left(int i) {
    __left = i;
}

void
Margin::right(int i) {
    __right = i;
}

int
Margin::top() const {
    return __top;
}

int
Margin::bottom() const {
    return __bottom;
}

int
Margin::left() const {
    return __left;
}

int
Margin::right() const {
    return __right;
}

Margin&
Margin::operator=(const Margin& m) {
    __top = m.__top;
    __bottom = m.__bottom;
    __left = m.__left;
    __right = m.__right;
    return *this;
}

Margin
Margin::operator+(const Margin& m) const {
    Margin tmp = *this;

    tmp.__top = max(__top, m.__top);
    tmp.__bottom = max(__bottom, m.__bottom);
    tmp.__left = max(__left, m.__left);
    tmp.__right = max(__right, m.__right);

    return tmp;
}

Margin
Margin::operator-(const Margin& m) const {
    Margin tmp = *this;

    tmp.__top = min(__top, m.__top);
    tmp.__bottom = min(__bottom, m.__bottom);
    tmp.__left = min(__left, m.__left);
    tmp.__right = min(__right, m.__right);

    return tmp;
}

bool
Margin::operator==(const Margin& m) const {
    if (this == &m) return true;

    return __top == m.__top &&
	__bottom == m.__bottom &&
	__left == m.__left &&
	__right == m.__right;
}

bool
Margin::operator!=(const Margin& m) const {
    return ! operator==(m);
}
