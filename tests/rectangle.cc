// $Id$
//
// Test Dimension class

#include "rectangle.h"

int main() {
    Rectangle<> o(0,0,25,80);
    Margin<> m(1,2,3,5);

    Rectangle<> r = o-m;

    if (r.rows() != 21 ||
	r.cols() != 73 ||
	r.y() != 1 ||
	r.x() != 2 )
	return 1;

    o = Rectangle<>(2,2,5,5);
    m = Margin<>(1,1,0,0);
    r = o-m;

    if (r.rows() != 4 &&
	r.cols() != 4)
	return 1;

    Rectangle<> o1(1,2,3,5);
    Rectangle<> o2(7,11,13,17);

    if (o1 == o2)
	return 1;

    if (o1 != o1)
	return 1;

    if (!(o1 == o1))
	return 1;

    o1 = o2;

    if (o1 != o2)
	return 1;

    if (!(o1 == o2))
	return 1;

    if (o1.rows() != 13 ||
	o1.cols() != 17)
	return 1;

    o1.rows(25);
    if (o1.rows() != 25 ||
	o1.cols() != 17 )
	return 1;

    o1.cols(80);
    if (o1.rows() != 25 ||
	o1.cols() != 80)
	return 1;

    o1.cols(80);
    if (o1.rows() != 25 ||
	o1.cols() != 80)
	return 1;

    o1.rows(25);
    if (o1.rows() != 25 ||
	o1.cols() != 80 )
	return 1;



    return 0;
}
