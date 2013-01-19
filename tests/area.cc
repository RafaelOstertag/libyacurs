// $Id$
//
// Test Dimension class

#include "area.h"

int main() {
    Size s1(7,11);
    if (s1.rows() != 7 ||
	s1.cols() != 11)
	return 1;

    Coordinates c1(13,17);
    if (c1.x()!=13 ||
	c1.y()!=17)
	return 1;

    Area o3(1,2,3,5);
    if (o3.x()!=2 ||
	o3.y()!=1 ||
	o3.rows()!=3 ||
	o3.cols()!=5)
	return 1;

    o3 = s1;
    if (o3.x()!=2 ||
	o3.y()!=1 ||
	o3.rows()!=7 ||
	o3.cols()!=11)
	return 1;

    o3 = c1;
    if (o3.x()!=13 ||
	o3.y()!=17 ||
	o3.rows()!=7 ||
	o3.cols()!=11)
	return 1;


    Area o(0,0,25,80);
    Margin m(1,2,3,5);

    Area r = o-m;

    if (r.rows() != 21 ||
	r.cols() != 73 ||
	r.y() != 1 ||
	r.x() != 2 )
	return 1;

    o = Area(2,2,6,6);
    m = Margin(1,1,0,0);
    r = o-m;

    if (r.rows() != 5 &&
	r.cols() != 5 &&
	r.x() != 3 &&
	r.y() != 3)
	return 1;

    r -= Margin(0,0,1,1);

    if (r.x() != 3 &&
	r.y() != 3 &&
	r.rows() != 4 &&
	r.cols() != 4)
	return 1;

    Area o1(1,2,3,5);
    Area o2(7,11,13,17);

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

    Area a1, a2(0,0,24,79);

    Margin m1(2,2,2,2);
    Margin m2(1,1,1,1);

    a1 = a2-m1;
    if (a1.x() != 2 ||
	a1.y() != 2 ||
	a1.rows() != 20 ||
	a1.cols() != 75)
	return 1;

    a1 = (a2-m1)-m2;
    if (a1.x() != 3 ||
	a1.y() != 3 ||
	a1.rows() != 18 ||
	a1.cols() != 73)
	return 1;

    return 0;
}
