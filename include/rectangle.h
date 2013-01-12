// -*- mode: c++ -*-

#ifndef RECTANGLE_H
#define RECTANGLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#include "coordinates.h"
#include "margin.h"

template<class T=int>
class Rectangle: public Coordinates<T> {
    private:
	Coordinates<T> c2;

    public:
	Rectangle() : Coordinates<T>(), c2() {}
	Rectangle(int _y1, int _x1, int _nrows, int _ncols) :
	    Coordinates<T>(_x1, _y1),
	    c2(_x1 + _ncols - 1, /* Transform to coords */
	       _y1 + _nrows - 1 /* Transform to coords */)  {
	    assert(this->x()>=0);
	    assert(this->y()>=0);
	    assert(c2.x()>=0);
	    assert(c2.y()>=0);
	}

	Rectangle(const Coordinates<T>& c, int _nrows=1, int _ncols=1) :
	    Coordinates<T>(c),
	    c2(c.x()+_ncols-1,c.y()+_nrows-1) {
	    assert(c2.x()>=0);
	    assert(c2.y()>=0);
	}

	Rectangle(const Rectangle<T>& d) : Coordinates<T>(d), c2(d.c2) {
	    assert(this->x()>=0);
	    assert(this->y()>=0);
	    assert(c2.x()>=0);
	    assert(c2.y()>=0);
	}

	~Rectangle() {}

	void rows(T l) {
	    assert(l>0);
	    c2.y(this->y() + l-1);
	}
	void cols(T c)  {
	    assert(c>0);
	    c2.x(this->x() + c-1);
	}

	T rows() const { return (c2 - *this).y() + 1; }
	T cols() const { return (c2 - *this).x() + 1; }

	Rectangle<T>& operator=(const Rectangle<T> &d) {
	    if (this == &d) return *this;

	    c2 = d.c2;
	    Coordinates<T>::operator=(d);

	    return *this;
	}

	bool operator==(const Rectangle<T>& d) const {
	    if (this == &d) return true;

	    return Coordinates<T>::operator==(*this) && c2 == d.c2;
	}

	bool operator!=(const Rectangle<T>& d) const {
	    return ! operator==(d);
	}

	bool operator==(const Coordinates<T>& d) const {
	    return false;
	}

	Rectangle<T>& operator-=(const Margin<T>& rhs) {
	    c2.y(c2.y() - rhs.bottom());
	    c2.x(c2.x() - rhs.right());
	    this->y(this->y() + rhs.top());
	    this->x(this->x() + rhs.left());
	    return *this;
	}

	Rectangle<T> operator-(const Margin<T>& rhs) const {
	    Rectangle<T> tmp = *this;
	    return tmp-=rhs;
	}

};

#endif
