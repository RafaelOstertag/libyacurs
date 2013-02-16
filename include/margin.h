// -*- mode: c++ -*-

#ifndef MARGIN_H
#define MARGIN_H

#include <string>

/**
 * Margin class
 */
class Margin {
    private:
	int __top, __bottom, __left, __right;

    public:
	Margin(int _top, int _left, int _bottom, int _right);
	Margin();
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
};

#endif
