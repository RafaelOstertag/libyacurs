// -*- mode: c++ -*-
//
// $Id$

#ifndef COORDINATES_H
#define COORDINATES_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

template<class T=int>
class Coordinates {
    private:
	T __x, __y;

    public:
	Coordinates() : __x(0), __y(0) {}
	Coordinates(T _x, T _y) : __x(_x), __y(_y) {}
	Coordinates(const Coordinates<T>& _c) {
	    __x = _c.__x;
	    __y = _c.__y;
	}
	virtual ~Coordinates() {}
	
	T x() const { return __x; }
	T y() const { return __y; }
	void x(T _x) { __x = _x; }
	void y(T _y) { __y = _y; }
	
	Coordinates<T>& operator=(const Coordinates<T>& rhs) {
	    __x = rhs.__x;
	    __y = rhs.__y;
	    return *this;
	}
	
	Coordinates<T>& operator+=(const Coordinates<T>& rhs) {
	    __x += rhs.__x;
	    __y += rhs.__y;
	    return *this;
	}

	Coordinates<T>& operator+=(const T& rhs) {
	    __x += rhs;
	    __y += rhs;
	    return *this;
	}
	
	Coordinates<T>& operator-=(const Coordinates<T>& rhs) {
	    __x -= rhs.__x;
	    __y -= rhs.__y;
	    return *this;
	}

	Coordinates<T>& operator-=(const T& rhs) {
	    __x -= rhs;
	    __y -= rhs;
	    return *this;
	}
	
	Coordinates<T> operator+(const Coordinates<T>& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp+=rhs;
	}

	Coordinates<T> operator+(const T& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp+=rhs;
	}

	Coordinates<T> operator-(const Coordinates<T>& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp-=rhs;
	}

	Coordinates<T> operator-(const T& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp-=rhs;
	}
	
	bool operator==(const Coordinates<T>& rhs) const {
	    return __x == rhs.__x && __y == rhs.__y;
	}

	bool operator!=(const Coordinates<T>& rhs) const {
	    return !operator==(rhs);
	}
	
};

#endif // COORDINATES_H
