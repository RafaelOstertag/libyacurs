// -*- mode: c++ -*-

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "coordinates.h"
#include "margin.h"

class Rectangle: public Coordinates {
    private:
	Coordinates c2;

    public:
	Rectangle();
	Rectangle(int _y, int _x, int _nrows, int _ncols);
	Rectangle(const Coordinates& c, int _nrows=1, int _ncols=1);
	Rectangle(const Rectangle& d);
	~Rectangle();

	void rows(int l);
	void cols(int c);

	int rows() const;
	int cols() const;

	Rectangle& operator=(const Rectangle &d);

	bool operator==(const Rectangle& d) const;
	bool operator!=(const Rectangle& d) const;
	bool operator==(const Coordinates& d) const;
	Rectangle& operator-=(const Margin& rhs);
	Rectangle operator-(const Margin& rhs) const;
};

#endif
