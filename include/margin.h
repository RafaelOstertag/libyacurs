// -*- mode: c++ -*-

#ifndef MARGIN_H
#define MARGIN_H

#include <string>

namespace YACURS {
    class Margin {
	private:
	    static Margin __zero;
	    int __top, __bottom, __left, __right;

	public:
	    Margin(int _top=0, int _left=0, int _bottom=0, int _right=0);
	    Margin(const Margin& m);
	    ~Margin();

	    void top(int i);
	    void bottom(int i);
	    void left(int i);
	    void right(int i);

	    int top() const;
	    int bottom() const;
	    int left() const;
	    int right() const;

	    Margin& operator=(const Margin& m);
	    Margin operator+(const Margin& m) const;
	    Margin operator-(const Margin& m) const;
	    bool operator==(const Margin& m) const;
	    bool operator!=(const Margin& m) const;

	    static const Margin& zero();
    };
}

#endif
