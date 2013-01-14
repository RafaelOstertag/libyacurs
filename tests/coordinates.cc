// $Id$
//
// Test Dimension class

#include "rectangle.h"

int main() {
    Coordinates o1(1,2);
    Coordinates o2(3,5);

    Coordinates r = o1-o2;

    if ( r.x() != -2 ||
	 r.y() != -3)
	return 1;

    if ( r != o1-o2 )
	return 1;

    if (!(r == o1-o2 ))
	return 1;

    o1 = o2;

    if ( o1 != o2 )
	return 1;

    if (!(o1 == o2))
	return 1;
	 
    o1 = r;

    if (o1 == o2)
	return 1;

    if (o1 != o1)
	return 1;

    if (!(o1 == o1))
	return 1;

    o1.x(2);
    o1.y(3);

    if ( o1.x() != 2 ||
	 o1.y() != 3 )
	 return 1;

    o2.y(5);
    o2.x(7);

    if ( o2.x() != 7 ||
	 o2.y() != 5 )
	 return 1;

    o1+=o2;

    if ( o1.x() != 9 ||
	 o1.y() != 8)
	return 1;

    if ( o2.x() != 7 ||
	 o2.y() != 5 )
	 return 1;

    o1 = o2 - Coordinates(2,3);

    if ( o1.x() != 5 ||
	 o1.y() != 2 )
	 return 1;

    if ( o2.x() != 7 ||
	 o2.y() != 5 )
	 return 1;
    return 0;
}
