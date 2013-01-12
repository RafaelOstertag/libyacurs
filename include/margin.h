// -*- mode: c++ -*-

#ifndef MARGIN_H
#define MARGIN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

template<class T=int>
class Margin {
    private:
	T __top, __bottom, __left, __right;

    protected:
	T max(T a, T b) const { return a>b ? a : b; }
	T min(T a, T b) const { return a<b ? a : b; }

    public:
	Margin(T t, T l, T b, T r) :
	    __top(t), __bottom(b), __left(l), __right(r) {}

	Margin():
	    __top(0), __bottom(0), __left(0), __right(0) {}

	Margin(const Margin<>& m) {
	    __top = m.__top;
	    __bottom = m.__bottom;
	    __left = m.__left;
	    __right = m.__right;
	}

	~Margin() {}

	void top(T i) { __top = i; }
	void bottom(T i) { __bottom = i; }
	void left(T i) { __left = i; }
	void right(T i) { __right = i; }

	T top() const { return __top; }
	T bottom() const { return __bottom; }
	T left() const { return __left; }
	T right() const { return __right; }

	Margin<T>& operator=(const Margin<T>& m) {
	    __top = m.__top;
	    __bottom = m.__bottom;
	    __left = m.__left;
	    __right = m.__right;
	    return *this;
	}

	Margin<T> operator+(const Margin<T>& m) const {
	    Margin<T> tmp = *this;

	    tmp.__top = max(__top, m.__top);
	    tmp.__bottom = max(__bottom, m.__bottom);
	    tmp.__left = max(__left, m.__left);
	    tmp.__right = max(__right, m.__right);

	    return tmp;
	}

	Margin<T> operator-(const Margin<T>& m) const {
	    Margin<T> tmp = *this;

	    tmp.__top = min(__top, m.__top);
	    tmp.__bottom = min(__bottom, m.__bottom);
	    tmp.__left = min(__left, m.__left);
	    tmp.__right = min(__right, m.__right);

	    return tmp;
	}

	bool operator==(const Margin<T>& m) const {
	    if (this == &m) return true;

	    return __top == m.__top &&
		__bottom == m.__bottom &&
		__left == m.__left &&
		__right == m.__right;
	}

	bool operator!=(const Margin<T>& m) const {
	    return ! operator==(m);
	}
};

#endif
