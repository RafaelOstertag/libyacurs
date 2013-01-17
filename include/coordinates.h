// -*- mode: c++ -*-
//
// $Id$

#ifndef COORDINATES_H
#define COORDINATES_H

/**
 * Coordinates as used throughout Curses.
 */
class Coordinates {
    private:
	int __y, __x;

    public:
	Coordinates(int _x=0, int _y=0);
	Coordinates(const Coordinates& _c);

	int x() const;
	int y() const;
	void x(int _x);
	void y(int _y);

	const Coordinates& operator=(const Coordinates& rhs);
	const Coordinates& operator+=(const Coordinates& rhs);
	const Coordinates& operator-=(const Coordinates& rhs);
	Coordinates operator+(const Coordinates& rhs) const;
	Coordinates operator-(const Coordinates& rhs) const;
	bool operator==(const Coordinates& rhs) const;
	bool operator!=(const Coordinates& rhs) const;

};

#endif // COORDINATES_H
