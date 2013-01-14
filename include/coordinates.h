// -*- mode: c++ -*-
//
// $Id$

#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates {
    private:
	int __y, __x;

    public:
	Coordinates();
	Coordinates(int _x, int _y);
	Coordinates(const Coordinates& _c);
	virtual ~Coordinates();

	int x() const;
	int y() const;
	void x(int _x);
	void y(int _y);

	Coordinates& operator=(const Coordinates& rhs);
	Coordinates& operator+=(const Coordinates& rhs);
	Coordinates& operator-=(const Coordinates& rhs);
	Coordinates operator+(const Coordinates& rhs) const;
	Coordinates operator-(const Coordinates& rhs) const;
	bool operator==(const Coordinates& rhs) const;
	bool operator!=(const Coordinates& rhs) const;

};

#endif // COORDINATES_H
